#include "UI/Inventory/InventorySlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Handler/ItemDragDropOperation.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryWidget.h"

void UInventorySlotWidget::InitSlot(
	int32 InIndex,const FInventorySlot& InSlot,const FDataItem* InRow,UInventoryWidget* InInventoryWidget)
{
	SlotIndex = InIndex;
	SlotData  = InSlot;
	ItemRow   = InRow;
	InventoryWidgetRef = InInventoryWidget;

	RefreshVisual();
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(
	const FGeometry&, const FPointerEvent& Event)
{
	if (SlotData.IsEmpty())
		return FReply::Unhandled();

	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(
			TakeWidget(), EKeys::LeftMouseButton);
	}

	if (bSplitModeActive &&
		Event.GetEffectingButton() == EKeys::RightMouseButton)
	{
		CancelSplit();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(
	const FGeometry&, const FPointerEvent&, UDragDropOperation*& OutOperation)
{
	auto* DragOp = NewObject<UItemDragDropOperation>(this);

	DragOp->ItemID        = SlotData.ItemID;
	DragOp->FromSlotIndex = SlotIndex;

	if (bSplitModeActive)
	{
		DragOp->DragType     = EDragType::DT_Split;
		DragOp->DragQuantity = SplitPreviewQuantity;
	}
	else
	{
		DragOp->DragType     = EDragType::DT_Normal;
		DragOp->DragQuantity = SlotData.Quantity;
	}

	if (DragVisualClass && ItemRow)
	{
		auto* Visual = CreateWidget<UDragItemVisual>(
			GetOwningPlayer(), DragVisualClass);

		Visual->ItemIcon->SetBrushFromTexture(
			ItemRow->ItemAsset.Icon);

		Visual->ItemQuantity->SetText(
			FText::AsNumber(DragOp->DragQuantity));

		DragOp->DefaultDragVisual = Visual;
		DragOp->Pivot = EDragPivot::CenterCenter;
	}

	// reset preview state
	bSplitModeActive = false;
	SplitPreviewQuantity = 0;
	OriginalQuantity = 0;

	OutOperation = DragOp;
}

bool UInventorySlotWidget::NativeOnDrop(
	const FGeometry&, const FDragDropEvent&, UDragDropOperation* InOperation)
{
	if (auto* DragOp = Cast<UItemDragDropOperation>(InOperation))
	{
		if (!InventoryWidgetRef)
			return false;

		// Drop ke slot yang sama → batal
		if (DragOp->FromSlotIndex == SlotIndex)
		{
			InventoryWidgetRef->RefreshInventory();
			return true;
		}

		InventoryWidgetRef->HandleSlotDrop(this, DragOp);
		return true;
	}
	return false;
}

FReply UInventorySlotWidget::NativeOnMouseWheel(
	const FGeometry&, const FPointerEvent& Event)
{
	if (SlotData.IsEmpty() || SlotData.Quantity <= 1)
		return FReply::Unhandled();

	const float Delta = Event.GetWheelDelta();
	if (Delta == 0.f)
		return FReply::Unhandled();

	if (!bSplitModeActive)
	{
		bSplitModeActive = true;
		OriginalQuantity = SlotData.Quantity;
		SplitPreviewQuantity = 1;
	}

	SplitPreviewQuantity += (Delta > 0 ? 1 : -1);
	SplitPreviewQuantity = FMath::Clamp(
		SplitPreviewQuantity, 1, OriginalQuantity - 1);

	// preview di slot asal
	ItemQuantity->SetText(
		FText::AsNumber(OriginalQuantity - SplitPreviewQuantity));

	return FReply::Handled();
}

void UInventorySlotWidget::RefreshVisual()
{
	if (!ItemIcon || !ItemQuantity)
		return;

	if (SlotData.IsEmpty() || !ItemRow)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantity->SetText(FText::GetEmpty());
		return;
	}

	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
	ItemQuantity->SetText(
		FText::AsNumber(SlotData.Quantity));
}

void UInventorySlotWidget::CancelSplit()
{
	bSplitModeActive     = false;
	SplitPreviewQuantity = 0;
	OriginalQuantity     = 0;

	RefreshVisual();
}

