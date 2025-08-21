// Inventory_Component.cpp


#include "CharacterComponent/Inventory_Component.h"

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

	// Cek stack dulu
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemID == ItemID)
		{
			Slot.Quantity += Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Item %s stacked. New Quantity: %d"), *ItemID.ToString(), Slot.Quantity);
			return true;
		}
	}

	// Cari slot kosong
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemID.IsNone())
		{
			Slot.ItemID = ItemID;
			Slot.Quantity = Quantity;
			UE_LOG(LogTemp, Warning, TEXT("Item %s added to empty slot. Quantity: %d"), *ItemID.ToString(), Quantity);
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Inventory full. Item %s could not be added."), *ItemID.ToString());
	return false;
}

