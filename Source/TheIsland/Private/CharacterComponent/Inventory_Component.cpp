// Inventory_Component.cpp


#include "CharacterComponent/Inventory_Component.h"
#include "Inventory/DataStruct/DataItem.h"
#include "Inventory/Item/BaseItem.h"

UInventory_Component::UInventory_Component()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	InventorySlotCapacity = 8;
	InventoryWeightCapacity = 50.f;

	//Inisiasi Slot Kosong
	InventoryContents.Init(nullptr, InventorySlotCapacity);
	
	OnInventoryUpdated.Broadcast();
}

UBaseItem* UInventory_Component::FindMatchingItem(
	UBaseItem* ItemIn) const
{

	if (!ItemIn) return nullptr;

	for (const TObjectPtr<UBaseItem>& InventoryItem : InventoryContents)
	{
		if (InventoryItem && InventoryItem->ID == ItemIn->ID)
			return InventoryItem;
	}
	return nullptr;
	
}

FItemAddResult UInventory_Component::HandleAddItem(UBaseItem* InputItem)
{

	if (!GetOwner() || !InputItem)
		return FItemAddResult::AddedNone(FText::FromString("Invalid item or no owner."));

	const int32 InitialRequestedAddAmount = InputItem->Quantity;

	// 🔹 Non-stackable
	if (!InputItem->ItemNumeric.bIsStackable)
		return HandleNonStackableItems(InputItem);

	// 🔹 Stackable
	const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

	if (StackableAmountAdded == InitialRequestedAddAmount)
	{
		return FItemAddResult::AddedAll(InitialRequestedAddAmount,
			FText::Format(FText::FromString("Successfully added {0} {1} to inventory."),
				InitialRequestedAddAmount,
				InputItem->ItemText.Name));
	}

	if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
	{
		return FItemAddResult::AddedPartial(InitialRequestedAddAmount,
			FText::Format(FText::FromString("Partial amount of {0} added. Number added = {1}"),
				InputItem->ItemText.Name,
				StackableAmountAdded));
	}

	return FItemAddResult::AddedNone(
		FText::Format(FText::FromString("Couldn't add {0}. Inventory full or invalid item."),
			InputItem->ItemText.Name));
}

UBaseItem* UInventory_Component::FindNextItemByID(
	UBaseItem* ItemIn) const
{

	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UBaseItem>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
	
}

UBaseItem* UInventory_Component::FindNextPartialStack(UBaseItem* ItemIn) const
{

	if (!ItemIn) return nullptr;

	const TObjectPtr<UBaseItem>* Result = InventoryContents.FindByPredicate(
		[&ItemIn](const UBaseItem* InventoryItem)
		{
			if (!InventoryItem) return false;
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsItemFullStack();
		});

	return Result ? *Result : nullptr;
}



void UInventory_Component::SplitExistingStack(
	UBaseItem* ItemIn, const int32 AmountToSplit)
{

	if (!ItemIn || AmountToSplit <= 0) return;

	if (HasEmptySlot())
	{
		// Kurangi jumlah dari item asli
		RemoveAmountOfItem(ItemIn, AmountToSplit);

		// Buat item copy baru untuk stack baru
		UBaseItem* NewStack = ItemIn->CreateItemCopy();
		if (!NewStack) return;

		NewStack->SetQuantity(AmountToSplit);
		AddNewItem(NewStack, AmountToSplit); // Broadcast sudah ada di AddNewItem
	}
	
	/*
	if (HasEmptySlot())
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
	*/
}

FItemAddResult UInventory_Component::HandleNonStackableItems(UBaseItem* InputItem)
{
	if (!InputItem)
		return FItemAddResult::AddedNone(FText::FromString("Invalid item reference."));

	if (InputItem->GetItemSingleWeight() < 0)
		return FItemAddResult::AddedNone(FText::FromString("Invalid item weight."));

	// 🔹 Check kapasitas berat
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > InventoryWeightCapacity)
	{
		return FItemAddResult::AddedNone(
			FText::Format(FText::FromString("Cannot add {0}. Exceeds weight capacity."),
				InputItem->ItemText.Name));
	}

	// 🔹 Check slot kosong
	if (!HasEmptySlot())
	{
		return FItemAddResult::AddedNone(
			FText::Format(FText::FromString("Cannot add {0}. No empty slot available."),
				InputItem->ItemText.Name));
	}

	// Tambahkan item
	AddNewItem(InputItem, 1);
	InventoryTotalWeight += InputItem->GetItemSingleWeight();

	return FItemAddResult::AddedAll(1,
		FText::Format(FText::FromString("Added single {0} successfully."),
			InputItem->ItemText.Name));
}

int32 UInventory_Component::HandleStackableItems(UBaseItem* ItemIn, int32 RequestedAddAmount)
{
	if (!ItemIn || RequestedAddAmount <= 0)
		return 0;

	UE_LOG(LogTemp, Warning, TEXT("HandleStackable: Item = %s | Quantity = %d | Stackable = %d | MaxStack = %d"),
		*ItemIn->ItemText.Name.ToString(),
		ItemIn->Quantity,
		ItemIn->ItemNumeric.bIsStackable,
		ItemIn->ItemNumeric.MaxStack);

	int32 AmountToDistribute = RequestedAddAmount;

	UBaseItem* ExistingItemStack = FindNextPartialStack(ItemIn);
	if (!ExistingItemStack)
	{
		UE_LOG(LogTemp, Warning, TEXT("No existing stack found for %s"), *ItemIn->ItemText.Name.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found partial stack for %s"), *ItemIn->ItemText.Name.ToString());
	}

	while (ExistingItemStack)
	{
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
		UE_LOG(LogTemp, Warning, TEXT("Stack found: Add %d to make full stack"), AmountToMakeFullStack);

		if (AmountToMakeFullStack > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + AmountToMakeFullStack);
			AmountToDistribute -= AmountToMakeFullStack;
			OnInventoryUpdated.Broadcast();
		}

		if (AmountToDistribute <= 0)
			return RequestedAddAmount;

		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	while (AmountToDistribute > 0 && GetEmptySlotCount() > 0)
	{
		const int32 AddAmount = FMath::Min(AmountToDistribute, ItemIn->ItemNumeric.MaxStack);
		UE_LOG(LogTemp, Warning, TEXT("Creating new stack of %d items"), AddAmount);

		UBaseItem* NewStack = ItemIn->CreateItemCopy();
		if (!NewStack)
		{
			UE_LOG(LogTemp, Error, TEXT("CreateItemCopy failed for %s!"), *ItemIn->ItemText.Name.ToString());
			return 0;
		}

		NewStack->SetQuantity(AddAmount);
		AddNewItem(NewStack, AddAmount);
		AmountToDistribute -= AddAmount;
	}

	UE_LOG(LogTemp, Warning, TEXT("End HandleStackableItems. Distributed = %d / %d"), 
		RequestedAddAmount - AmountToDistribute, RequestedAddAmount);

	return RequestedAddAmount - AmountToDistribute;
}

int32 UInventory_Component::CalculateWeightAddAmount(UBaseItem* ItemIn,int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((InventoryWeightCapacity - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	return FMath::Min(WeightMaxAddAmount, RequestedAddAmount);
}

int32 UInventory_Component::CalculateNumberForFullStack(
	UBaseItem* StackableItem,int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumeric.MaxStack - StackableItem->Quantity;
	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventory_Component::AddNewItem(UBaseItem* Item, const int32 AmountToAdd)
{
	if (!Item) return;

	UBaseItem* NewItem = (Item->bIsCopy || Item->bIsPickup) ? Item : Item->CreateItemCopy();
	NewItem->ResetItemFlag();
	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	bool bAdded = false;
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i] == nullptr)
		{
			InventoryContents[i] = NewItem;
			bAdded = true;
			break;
		}
	}

	if (!bAdded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory full, cannot add new item!"));
		return;
	}

	InventoryTotalWeight += NewItem->GetItemSingleWeight();

	// 🔹 Debug info
	UE_LOG(LogTemp, Warning, TEXT("Added %s | Empty slots left: %d"),
		*NewItem->ItemText.Name.ToString(),
		GetEmptySlotCount());

	OnInventoryUpdated.Broadcast();
}

int32 UInventory_Component::
GetEmptySlotCount() const
{
	int32 EmptyCount = 0;
	for (UBaseItem* Slot : InventoryContents)
	{
		if (Slot == nullptr)
			EmptyCount++;
	}
	return EmptyCount;
}

bool UInventory_Component::HasEmptySlot() const
{
	for (UBaseItem* Slot : InventoryContents)
	{
		if (Slot == nullptr)
			return true;
	}
	return false;
}

void UInventory_Component::RemoveSingleInstanceOfItem(
	UBaseItem* ItemToRemove)
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i] == ItemToRemove)
		{
			InventoryContents[i] = nullptr;
			break;
		}
	}
	OnInventoryUpdated.Broadcast();
}

int32 UInventory_Component::RemoveAmountOfItem(
	UBaseItem* ItemIn,int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove*ItemIn -> GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

