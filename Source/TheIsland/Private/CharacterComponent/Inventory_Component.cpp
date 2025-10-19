// Inventory_Component.cpp


#include "CharacterComponent/Inventory_Component.h"
#include "Inventory/DataStruct/BaseItem.h"

UInventory_Component::UInventory_Component():
	ItemDataTable(nullptr)

{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventory_Component::BeginPlay()
{
	Super::BeginPlay();

	InitializeInventory(8); // 8 slot
	
	AddItem("Wood_01", 5);
	AddItem("Apple_01", 2);
	AddItem("Wood_01", 3); // harus stack ke slot pertama
}

void UInventory_Component::InitializeInventory(int32 Size)
{
	Slots.SetNum(Size);
}

bool UInventory_Component::AddItem(FName ItemID, int32 Quantity)
{
	if (Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Quantity: %d"), Quantity);
		return false;
	}

	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem failed: ItemDataTable is null"));
		return false;
	}

	const FBaseItem* ItemData = ItemDataTable->FindRow<FBaseItem>(ItemID, TEXT("AddItem"));
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem failed: Item %s not found in DataTable"), *ItemID.ToString());
		return false;
	}

	int32 MaxStack = ItemData->ItemNumeric.MaxStack > 0 ? ItemData->ItemNumeric.MaxStack : 1;
	int32 Remaining = Quantity;

	// Stack ke slot yang sudah ada
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemID == ItemID && Slot.Quantity < MaxStack)
		{
			int32 Space = MaxStack - Slot.Quantity;
			int32 AddAmount = FMath::Min(Space, Remaining);
			Slot.Quantity += AddAmount;
			Remaining -= AddAmount;

			UE_LOG(LogTemp, Warning, TEXT("Item %s stacked. Added %d. New Quantity: %d"), 
				*ItemID.ToString(), AddAmount, Slot.Quantity);

			if (Remaining <= 0) break;
		}
	}

	// Masukkan ke slot kosong
	if (Remaining > 0)
	{
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.ItemID.IsNone())
			{
				int32 AddAmount = FMath::Min(MaxStack, Remaining);
				Slot.ItemID = ItemID;
				Slot.Quantity = AddAmount;
				Remaining -= AddAmount;

				UE_LOG(LogTemp, Warning, TEXT("Item %s added to empty slot. Quantity: %d"), 
					*ItemID.ToString(), AddAmount);

				if (Remaining <= 0) break;
			}
		}
	}

	// Inventory penuh
	if (Remaining > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory full. %d items of %s could not be added."), Remaining, *ItemID.ToString());
	}

	// Broadcast update
	OnInventoryUpdated.Broadcast();

	return (Remaining < Quantity); // true jika ada yang berhasil ditambahkan
}