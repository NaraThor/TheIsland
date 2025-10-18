// Inventory_Component.cpp


#include "CharacterComponent/Inventory_Component.h"

#include "Inventory/DataStruct/BaseItem.h"

UInventory_Component::UInventory_Component()
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

	// 🔹 1️⃣ Coba cari slot untuk item ini (baik stackable atau kosong)
	FInventorySlot* TargetSlot = FindAvailableSlot(ItemID);
	if (!TargetSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory full. Item %s could not be added."), *ItemID.ToString());
		return false;
	}

	// 🔹 2️⃣ Tambah quantity
	if (TargetSlot->ItemID.IsNone())
	{
		// Slot kosong → isi baru
		TargetSlot->ItemID = ItemID;
		TargetSlot->Quantity = Quantity;
		UE_LOG(LogTemp, Warning, TEXT("Item %s added to empty slot. Quantity: %d"), *ItemID.ToString(), Quantity);
	}
	else
	{
		// Slot sudah berisi item sama → stack
		TargetSlot->Quantity += Quantity;
		UE_LOG(LogTemp, Warning, TEXT("Item %s stacked. New Quantity: %d"), *ItemID.ToString(), TargetSlot->Quantity);
	}

	// 🔹 3️⃣ Broadcast update hanya sekali di akhir
	OnInventoryUpdated.Broadcast();
	return true;
}

FInventorySlot* UInventory_Component::FindAvailableSlot(FName ItemID)
{
	if (!ItemDataTable) return nullptr;

	const FBaseItem* ItemData = ItemDataTable->FindRow<FBaseItem>(ItemID, TEXT("FindAvailableSlot"));
	if (!ItemData) return nullptr;

	int32 MaxStack = ItemData->MaxStack;

	// Cari slot dengan item sama yang belum full
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemID == ItemID && Slot.Quantity < MaxStack)
			return &Slot;
	}

	// Cari slot kosong
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemID.IsNone())
			return &Slot;
	}

	return nullptr;
}



