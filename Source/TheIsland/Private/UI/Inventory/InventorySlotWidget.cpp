// InventorySlotWidget.cpp
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Handler/ItemDragDropOperation.h"
#include "Inventory/Item/BaseItem.h"
#include "UI/Inventory/DragItemVisual.h"

//DragDrop
void UInventorySlotWidget::SetItemData(const FInventorySlot& InSlot, const FDataItem* Row)
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

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
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
	DragOp->ItemID = SlotData.ItemID;           // WAJIB
	DragOp->DragQuantity = SlotData.Quantity;   // opsional tapi berguna
	DragOp->SlotData = SlotData;                // salin full struct (opsional)
	DragOp->SlotIndex = SlotIndex;              // WAJIB: index yang kita set dari InventoryWidget
	// SourceInventory bisa diisi jika kamu mau:
	// DragOp->SourceInventory = Owning inventory pointer if available
	
	// Buat visual drag jika kamu punya widget classnya (opsional)
	if (DragItemVisualClass)
	{
		UDragItemVisual* Visual = CreateWidget<UDragItemVisual>(GetOwningPlayer(), DragItemVisualClass);
		if (Visual)
		{
			// contoh set icon/amount di visual jika ada binding
			// Visual->SetIcon( ... );
			DragOp->DefaultDragVisual = Visual;
		}
	}

	OutOperation = DragOp;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	// Jangan handle di slot; biarkan parent InventoryWidget yang menangani swap/move.
	// Jika kamu ingin slot menangani drop untuk stack merge atau swap langsung,
	// implementasikan di sini dan return true untuk consume.
	return false;
}