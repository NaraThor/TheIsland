#include "UI/Inventory/InventoryWidget.h"

#include "Character/CharacterOrigin.h"
#include "CharacterComponent/Inventory_Component.h"
#include "Components/WrapBox.h"
#include "Handler/ItemDragDropOperation.h"
#include "Inventory/Item/BaseItem.h"
#include "UI/Inventory/InventorySlotWidget.h"


void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACharacterOrigin>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;

	InventoryReference = PlayerCharacter->GetInventory();
	if (!InventoryReference) return;

	InventoryReference->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::RefreshInventory);

	// Debug log
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget Constructed: %s | Parent: %s | InViewport: %d | Outer: %s | OwningPawn: %s"),
		*GetNameSafe(this),
		*GetNameSafe(GetParent()),
		IsInViewport() ? 1 : 0,
		*GetNameSafe(GetOuter()),
		*GetNameSafe(GetOwningPlayerPawn()));

	UE_LOG(LogTemp, Warning, TEXT("UI Refresh From Inventory Work!!! %s"), *GetName());
}


void UInventoryWidget::RefreshInventory()
{

	if (!InventoryReference || !InventorySlotClass) return;

	InventoryPanel->ClearChildren();

	// Ambil semua slot (termasuk yang kosong)
	const TArray<FInventorySlot>& Slots = InventoryReference->GetInventoryContents();

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		const FInventorySlot& InventorySlot = Slots[i];

		const FDataItem* ItemRow = nullptr;
		if (!InventorySlot.IsEmpty())
		{
			ItemRow = InventoryReference->GetItemRow(InventorySlot.ItemID);
		}

		UInventorySlotWidget* ItemSlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
		if (!ItemSlotWidget) continue;

		// SET SLOT INDEX DI SINI
		ItemSlotWidget->SetSlotIndex(i);

		// Serahkan data ke widget (Row bisa nullptr → otomatis dianggap slot kosong)
		ItemSlotWidget->SetItemData(InventorySlot, ItemRow);

		// Tambahkan ke panel UI
		InventoryPanel->AddChild(ItemSlotWidget);
	}
}

bool UInventoryWidget::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if (!ItemDragDrop || !InventoryPanel)
		return false;

	FVector2D DropPosScreen = InDragDropEvent.GetScreenSpacePosition();
	bool bInside = InventoryPanel->GetCachedGeometry().IsUnderLocation(DropPosScreen);

	if (bInside)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop INSIDE Inventory Panel"));
		//TODO: lakukan swap/move/merge berdasarkan ItemDragDrop->SlotIndex dan target slot

		// Contoh pseudocode:
		// int32 SourceIndex = ItemDragDrop->SlotIndex;
		// int32 TargetIndex = DetermineTargetSlotIndexFromPosition(DropPosScreen);
		// InventoryReference->MoveOrSwap(SourceIndex, TargetIndex);

		return true; // Inventory menangani drop
	}

	// Drop di luar → biarkan bubble ke MainMenu
	return false;
}

