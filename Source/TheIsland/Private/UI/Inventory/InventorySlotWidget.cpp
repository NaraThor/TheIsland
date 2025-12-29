#include "UI/Inventory/InventorySlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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

	// ===== CANCEL scroll drag jika left click normal =====
	if (bSplitModeActive && Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		CancelSplit(); // hilangkan drag visual scroll
		if (InventoryWidgetRef && InventoryWidgetRef->DraggedSlot == this)
		{
			InventoryWidgetRef->DraggedSlot = nullptr;
		}
		// Lanjut ke detect drag normal di bawah
	}

	// LEFT CLICK → start normal drag
	if (Event.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	// RIGHT CLICK → cancel scroll drag
	if (Event.GetEffectingButton() == EKeys::RightMouseButton && bSplitModeActive)
	{
		CancelSplit();
		if (InventoryWidgetRef && InventoryWidgetRef->DraggedSlot == this)
		{
			InventoryWidgetRef->DraggedSlot = nullptr;
		}
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

// === MARK: Scroll pick (drag) ===
FReply UInventorySlotWidget::NativeOnMouseWheel(
	const FGeometry&, const FPointerEvent& Event)
{
	if (SlotData.IsEmpty() || SlotData.Quantity <= 1)
		return FReply::Unhandled();

	float Delta = Event.GetWheelDelta();
	if (Delta == 0.f)
		return FReply::Unhandled();

	// Jika tidak ada drag aktif atau drag sedang di slot lain → switch drag ke slot ini
	if (!InventoryWidgetRef->DraggedSlot || InventoryWidgetRef->DraggedSlot != this)
	{
		// Cancel drag sebelumnya
		if (InventoryWidgetRef->DraggedSlot)
		{
			InventoryWidgetRef->DraggedSlot->CancelSplit();
		}

		// Set slot ini sebagai drag baru
		InventoryWidgetRef->DraggedSlot = this;

		bSplitModeActive = true;
		OriginalQuantity = SlotData.Quantity;
		SplitPreviewQuantity = 1;

		// Buat drag visual baru
		if (DragVisualClass && ItemRow)
		{
			if (SplitVisual)
				SplitVisual->RemoveFromParent(); // hapus visual lama

			SplitVisual = CreateWidget<UDragItemVisual>(GetOwningPlayer(), DragVisualClass);
			if (SplitVisual)
			{
				SplitVisual->ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
				SplitVisual->ItemQuantity->SetText(FText::AsNumber(SplitPreviewQuantity));
				SplitVisual->AddToViewport(9999); // pastikan selalu di atas
			}
		}

		// Preview slot asal
		ItemQuantity->SetText(FText::AsNumber(OriginalQuantity - SplitPreviewQuantity));
	}
	else
	{
		// Scroll di slot yang sama → adjust quantity
		SplitPreviewQuantity += (Delta > 0 ? 1 : -1);
		SplitPreviewQuantity = FMath::Clamp(SplitPreviewQuantity, 1, OriginalQuantity - 1);

		if (SplitVisual)
			SplitVisual->ItemQuantity->SetText(FText::AsNumber(SplitPreviewQuantity));

		ItemQuantity->SetText(FText::AsNumber(OriginalQuantity - SplitPreviewQuantity));
	}

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


	// Hapus drag visual dari viewport
	if (SplitVisual)
	{
		SplitVisual->RemoveFromParent();
		SplitVisual = nullptr; // reset pointer
	}
	
	RefreshVisual();
}

// === MARK: Tick untuk drag visual mengikuti cursor ===
void UInventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SplitVisual)
	{
		float MouseX = 0.f;
		float MouseY = 0.f;

		// Ambil PlayerController
		if (APlayerController* PC = GetOwningPlayer())
		{
			// Dapatkan posisi mouse dalam viewport (scaled by DPI)
			if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PC, MouseX, MouseY))
			{
				FVector2D MousePos(MouseX, MouseY);
				SplitVisual->SetPositionInViewport(MousePos, true); // true = bRemoveDPIScale
			}
		}
	}
}
