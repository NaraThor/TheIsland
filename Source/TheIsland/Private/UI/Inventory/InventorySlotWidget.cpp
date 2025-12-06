// InventorySlotWidget.cpp
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Handler/ItemDragDropOperation.h"
#include "Inventory/Item/BaseItem.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryWidget.h"

//DragDrop
void UInventorySlotWidget::SetItemData(
	const FInventorySlot& InSlot, const FDataItem* Row)
{
	SlotData = InSlot; // assign ke member
	ItemRow = Row;

	// Refresh visual
	RefreshVisual();
}

void UInventorySlotWidget::RefreshVisual()
{
	if (!ItemIcon || !ItemQuantity || !ItemBorder)
		return;

	// SLOT KOSONG
	if (!ItemRow || SlotData.IsEmpty())
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor::Yellow);
		return;
	}

	// WARNA BORDER -> contoh simple
	switch (ItemRow->ItemNumeric.IsStackable() ? EItemRarityData::Common : EItemRarityData::Rare)
	{
	case EItemRarityData::Common:    ItemBorder->SetBrushColor(FLinearColor::Gray); break;
	case EItemRarityData::Rare:      ItemBorder->SetBrushColor(FLinearColor::White); break;
	case EItemRarityData::Epic:      ItemBorder->SetBrushColor(FLinearColor(0.0f,0.71f,0.169f)); break;
	case EItemRarityData::Mythic:    ItemBorder->SetBrushColor(FLinearColor(0.0f,0.4f,0.75f)); break;
	case EItemRarityData::Legendary: ItemBorder->SetBrushColor(FLinearColor(1.0f,0.45f,0.0f)); break;
	}

	// ICON
	ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);

	// QUANTITY
	if (ItemRow->ItemNumeric.IsStackable())
	{
		int32 QtyToShow = (PreviewQuantity >= 0) ? PreviewQuantity: SlotData.Quantity;
		ItemQuantity->SetText(FText::AsNumber(SlotData.Quantity));
		ItemQuantity->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	}
}


// ----------------------------------------------------------
// NativeConstruct
// ----------------------------------------------------------
void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshVisual(); // pertama kali
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,const FPointerEvent& InMouseEvent)
{

	bIsSplitDrag = FSlateApplication::IsInitialized() &&
				   FSlateApplication::Get().GetModifierKeys().IsControlDown();

	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);

}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(
	const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// Cek slot kosong
	if (SlotData.IsEmpty())
	{
		OutOperation = nullptr;
		return;
	}

	// Buat operasi drag
	UItemDragDropOperation* DragOp = NewObject<UItemDragDropOperation>(this);
	
	//Data
	DragOp->ItemID			= SlotData.ItemID;				// ID Item
	DragOp->SlotData		= SlotData;						// Salin full struct (opsional)
	DragOp->SlotIndex		= SlotIndex;					// WAJIB: index yang kita set dari InventoryWidget
	DragOp->SourceInventory = OwningInventory;				// Jika tersedia
	DragOp->SourceInventorySlotWidget = this;
	// SourceInventory bisa diisi jika kamu mau:
	// DragOp->SourceInventory = Owning inventory pointer if available

	//Dragtype
	if (InMouseEvent.IsControlDown() && SlotData.Quantity > 1)
	{
		// SPLIT DRAG
		int32 CalculatedSplit = FMath::CeilToInt(SlotData.Quantity / 2.0f);

		DragOp->DragType = EDragType::DT_Split;
		DragOp->bIsSplitDrag = true;
		DragOp->SplitAmount = CalculatedSplit;
		DragOp->DragQuantity = CalculatedSplit;

		// Kurangi slot asal
		const int32 Remaining = SlotData.Quantity - DragOp->DragQuantity;
		SlotData.Quantity = Remaining;

		if (Remaining <= 0)
			SlotData.Clear();

		RefreshVisual();
	}
	else
	{
		// NORMAL DRAG: bawa seluruh slot
		DragOp->DragType = EDragType::DT_Normal;
		DragOp->bIsSplitDrag = false;
		DragOp->SplitAmount = 0;
		DragOp->DragQuantity = SlotData.Quantity;
	}
	
	
	// Buat visual drag jika kamu punya widget classnya (opsional)
	if (DragItemVisualClass && ItemRow)
	{
		UDragItemVisual* Visual = CreateWidget<UDragItemVisual>(GetOwningPlayer(), DragItemVisualClass);
		if (Visual)
		{
			// Set icon & border
			Visual->ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
			Visual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

			// Set jumlah jika stackable
			if (ItemRow->ItemNumeric.IsStackable())
			{
				// pastikan DragOp valid sebelum akses
				Visual->ItemQuantity->SetText(FText::AsNumber(DragOp->DragQuantity));
				Visual->ItemQuantity->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Visual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
			}


			// Assign visual ke drag operation
			DragOp->DefaultDragVisual = Visual;
			DragOp->Pivot = EDragPivot::CenterCenter;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("OnDragDetected: Type=%d DragQty=%d Split=%d"),
	(int)DragOp->DragType, DragOp->DragQuantity, DragOp->SplitAmount);
	
	OutOperation = DragOp;
}

bool UInventorySlotWidget::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("[SLOT] OnDrop masuk ke SlotIndex = %d"), SlotIndex);

	if (UItemDragDropOperation* DragOp = Cast<UItemDragDropOperation>(InOperation))
	{
		UE_LOG(LogTemp, Warning, TEXT("[SLOT] DragOp valid! ItemID = %s, Qty = %d, SourceIndex = %d"),
			*DragOp->ItemID.ToString(),DragOp->DragQuantity,DragOp->SlotIndex);

		//panggil inventory widget
		if (InventoryWidgetRef)
		{
			InventoryWidgetRef->HandleSlotDrop(this, DragOp);
		}

		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("[SLOT] DragOp TIDAK VALID"));
	return false;
}

void UInventorySlotWidget::UpdateSlotAfterSplit(int32 NewQuantity)
{
	SlotData.Quantity = NewQuantity;
	if (SlotData.Quantity <= 0)
	{
		SlotData.Clear();
	}
	RefreshVisual();
}

bool UInventorySlotWidget::NativeOnDragOver(
	const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UItemDragDropOperation* DragOp = Cast<UItemDragDropOperation>(InOperation))
	{
		if (DragOp->DragType == EDragType::DT_Split)
		{
			PreviewQuantity = DragOp->SplitAmount; // hanya preview
			RefreshVisual();
			return true;
		}
	}

	return false;
}

void UInventorySlotWidget::NativeOnDragLeave(
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	PreviewQuantity = -1; // reset preview
	RefreshVisual();
}