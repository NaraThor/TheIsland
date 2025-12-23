#include "UI/Inventory/InventorySlotWidget.h"

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

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(
	const FGeometry& InGeometry, const FPointerEvent& Event, UDragDropOperation*& OutOperation)
{
	if (SlotData.IsEmpty())
		return;

	// Buat DragDropOperation custom
	auto* DragOp = NewObject<UItemDragDropOperation>(this);

	DragOp->ItemID        = SlotData.ItemID;
	DragOp->FromSlotIndex = SlotIndex;

	// Tentukan jumlah drag & tipe (normal atau split)
	if (Event.IsControlDown() && SlotData.Quantity > 1)
	{
		// Split drag, ambil setengah
		const int32 SplitQty = FMath::CeilToInt(SlotData.Quantity / 2.f);
		DragOp->DragType     = EDragType::DT_Split;
		DragOp->bIsSplitDrag = true;
		DragOp->DragQuantity = SplitQty;

		// Kurangi slot sumber secara langsung
		SlotData.Quantity -= SplitQty;
	}
	else
	{
		// Normal drag, ambil semua
		DragOp->DragType     = EDragType::DT_Normal;
		DragOp->DragQuantity = SlotData.Quantity;

		// Hilangkan semua dari slot
		SlotData.Quantity = 0;
	}

	// Refresh slot visual agar slot asal update
	RefreshVisual();

	// --- Buat drag visual ---
	if (DragVisualClass && ItemRow) // Pastikan class & item valid
	{
		UDragItemVisual* Visual = CreateWidget<UDragItemVisual>(GetOwningPlayer(), DragVisualClass);
		if (Visual)
		{
			Visual->ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
			Visual->ItemQuantity->SetText(FText::AsNumber(DragOp->DragQuantity));

			DragOp->DefaultDragVisual = Visual;
			DragOp->Pivot = EDragPivot::CenterCenter;

			// Simpan pointer ke visual agar bisa diupdate realtime
			DragOp->DragVisual = Visual;
		}
	}

	// Kirim operation ke engine
	OutOperation = DragOp;
}


bool UInventorySlotWidget::NativeOnDrop(
	const FGeometry&,const FDragDropEvent&,UDragDropOperation* InOperation)
{
	if (auto* DragOp =
		Cast<UItemDragDropOperation>(InOperation))
	{
		if (InventoryWidgetRef)
		{
			InventoryWidgetRef->HandleSlotDrop(this, DragOp);
			return true;
		}
	}
	return false;
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
