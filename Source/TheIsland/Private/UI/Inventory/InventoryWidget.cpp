#include "UI/Inventory/InventoryWidget.h"
#include "CharacterComponent/Inventory_Component.h"
#include "Components/HorizontalBox.h"
#include "UI/Inventory/InventorySlotWidget.h"

void UInventoryWidget::RefreshInventory(UInventory_Component* InventoryRef)
{
	if (!InventoryRef || !SlotWidgetClass || !HorizontalPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshInventory failed: Missing reference or class"));
		return;
	}

	// Hapus isi panel sebelum menggambar ulang
	HorizontalPanel->ClearChildren();

	// Loop semua slot dari komponen inventory
	for (const FInventorySlot& SlotData : InventoryRef->Slots)
	{
		UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (NewSlot)
		{
			NewSlot->UpdateSlot(SlotData);
			HorizontalPanel->AddChild(NewSlot);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget refreshed: %d slots."), InventoryRef->Slots.Num());
}