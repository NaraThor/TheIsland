#include "UI/Inventory/InventorySlotWidget.h"

// UE
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/Reply.h"

// Inventory
#include "Inventory/DataStruct/DataItem.h"
#include "Handler/ItemDragDropOperation.h"

// UI
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// =====================================================
// INIT SLOT
// =====================================================
// =====================================================
// INIT SLOT
// =====================================================
void UInventorySlotWidget::InitSlot(
	int32 InIndex,
	const FInventorySlot& InSlot,
	const FDataItem* InRow,
	UInventoryWidget* InInventoryWidget)
{
	SlotIndex          = InIndex;
	SlotData           = InSlot;
	ItemRow            = InRow;
	InventoryWidgetRef = InInventoryWidget;

	// Reset modifier state (AMAN)
	bShiftHeld    = false;
	bRightAltHeld = false;

	RefreshVisual();
}

// =====================================================
// MOUSE BUTTON DOWN → PREPARE DRAG
// =====================================================
FReply UInventorySlotWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (SlotData.IsEmpty())
		return FReply::Unhandled();

	bShiftHeld    = InMouseEvent.IsShiftDown();
	bRightAltHeld = InMouseEvent.IsRightAltDown();

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(
			TakeWidget(),
			EKeys::LeftMouseButton
		);
	}

	return FReply::Unhandled();
}

// =====================================================
// DRAG DETECTED
// =====================================================
void UInventorySlotWidget::NativeOnDragDetected(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	UItemDragDropOperation* DragOp = NewObject<UItemDragDropOperation>(this);
	DragOp->ItemID        = SlotData.ItemID;
	DragOp->FromSlotIndex = SlotIndex;

	// Tentukan jumlah yang di-drag
	int32 QuantityToDrag;
	if (bRightAltHeld) // Split One
	{
		QuantityToDrag = FMath::Min(1, SlotData.Quantity);
		DragOp->DragType = (SlotData.Quantity <= 1) ? EDragType::DT_Normal : EDragType::DT_SplitOne;
	}
	else if (bShiftHeld) // Half Split
	{
		QuantityToDrag = (SlotData.Quantity <= 1) ? SlotData.Quantity : FMath::Max(1, SlotData.Quantity / 2);
		DragOp->DragType = (SlotData.Quantity <= 1) ? EDragType::DT_Normal : EDragType::DT_HalfSplit;
	}
	else // Normal drag
	{
		QuantityToDrag = SlotData.Quantity;
		DragOp->DragType = EDragType::DT_Normal;
	}

	DragOp->DragQuantity = QuantityToDrag;

	// Kurangi slot asli sesuai drag → slot langsung update
	SlotData.Quantity -= QuantityToDrag;
	if (SlotData.Quantity <= 0)
	{
		SlotData.ItemID = NAME_None;
		SlotData.Quantity = 0;
	}
	RefreshVisual();

	// Buat temporary slot untuk drag visual → pakai QuantityToDrag
	FInventorySlot TempSlot;
	TempSlot.ItemID   = ItemRow->ID;       // gunakan ID item yang sedang di-drag
	TempSlot.Quantity = QuantityToDrag;    // pakai jumlah yang di-drag

	// Buat drag visual
	if (DragVisualClass)
	{
		UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(GetOwningPlayer(), DragVisualClass);
		if (DragVisual)
		{
			DragVisual->InitVisual(TempSlot, ItemRow);
			DragOp->DefaultDragVisual = DragVisual;
			DragOp->Pivot = EDragPivot::MouseDown;
		}
	}

	// Kirim drag operation
	OutOperation = DragOp;

	// Reset modifier
	bShiftHeld = false;
	bRightAltHeld = false;
}

void UInventorySlotWidget::NativeOnDragCancelled(
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UItemDragDropOperation* DragOp =
		Cast<UItemDragDropOperation>(InOperation);

	if (!DragOp || !InventoryWidgetRef)
		return;

	// kalau ternyata drop ke slot, JANGAN spawn world
	if (DragOp->bDroppedOnSlot)
		return;

	// =============================
	// REAL DROP KE WORLD
	// =============================
	InventoryWidgetRef->DropItemToWorld(
		DragOp->FromSlotIndex,
		DragOp->DragQuantity
	);

	// Refresh UI
	InventoryWidgetRef->RefreshInventory();
}

// =====================================================
// DROP
// =====================================================
bool UInventorySlotWidget::NativeOnDrop(
	const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UItemDragDropOperation* DragOp =
		Cast<UItemDragDropOperation>(InOperation);

	if (!DragOp || !InventoryWidgetRef)
		return false;

	DragOp->bDroppedOnSlot = true;
	
	InventoryWidgetRef->HandleSlotDrop(this, DragOp);
	return true;
}

// =====================================================
// REFRESH VISUAL
// =====================================================
void UInventorySlotWidget::RefreshVisual()
{
	if (!ItemIcon || !ItemQuantity)
		return;

	if (!ItemRow || SlotData.IsEmpty())
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantity->SetText(FText::GetEmpty());
		return;
	}

	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
	ItemQuantity->SetText(FText::AsNumber(SlotData.Quantity));
}

