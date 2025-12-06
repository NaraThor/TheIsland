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
	RefreshInventory();
}


void UInventoryWidget::RefreshInventory()
{

	if (!InventoryReference || !InventorySlotClass) return;

	InventoryPanel->ClearChildren();

	const TArray<FInventorySlot>& Slots = InventoryReference->GetInventoryContents();

	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); SlotIndex++)
	{
		const FInventorySlot& InventorySlot = Slots[SlotIndex];

		// Ambil row jika slot tidak kosong
		const FDataItem* ItemRow = nullptr;
		if (!InventorySlot.IsEmpty())
		{
			ItemRow = InventoryReference->GetItemRow(InventorySlot.ItemID);
		}

		UInventorySlotWidget* ItemSlotWidget =
			CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
		if (!ItemSlotWidget) continue;

		// Set slot index (lebih jelas sekarang)
		ItemSlotWidget->SetSlotIndex(SlotIndex);

		// Assign reference
		ItemSlotWidget->InventoryWidgetRef = this;
		ItemSlotWidget->OwningInventory   = InventoryReference;

		// Set data item
		ItemSlotWidget->SetItemData(InventorySlot, ItemRow);

		// Tambahkan widget ke panel
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

void UInventoryWidget::HandleSlotDrop(
	UInventorySlotWidget* DropTargetSlot,UItemDragDropOperation* DragOp)
{
	if (!InventoryReference || !DragOp) return;

    const int32 FromIndex = DragOp->SlotIndex;
    const int32 ToIndex   = DropTargetSlot->SlotIndex;

    // Case 1: SAME SLOT → ignore
    if (FromIndex == ToIndex)
        return;

    switch (DragOp->DragType)
    {
        case EDragType::DT_Normal:
            InventoryReference->MoveSlotToSlot(FromIndex, ToIndex);
            break;

        case EDragType::DT_Split:
            InventoryReference->SplitItem(FromIndex, ToIndex, DragOp->DragQuantity);
            break;

        default:
            break;
    }

    RefreshInventory();
}