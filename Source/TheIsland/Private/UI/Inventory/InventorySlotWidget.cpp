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
	NativeConstruct();
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ItemRow || SlotData.IsEmpty())
	{
		// Kosongkan slot
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor(0.1f,0.1f,0.1f,0.2f));
		return;
	}
	// --- Atur warna border berdasarkan rarity ---
	switch (ItemRow->ItemNumeric.IsStackable() ? EItemRarityData::Common : EItemRarityData::Rare) // contoh simple
	{
	case EItemRarityData::Common:    ItemBorder->SetBrushColor(FLinearColor::Gray); break;
	case EItemRarityData::Rare:      ItemBorder->SetBrushColor(FLinearColor::White); break;
	case EItemRarityData::Epic:      ItemBorder->SetBrushColor(FLinearColor(0.0f,0.71f,0.169f)); break;
	case EItemRarityData::Mythic:    ItemBorder->SetBrushColor(FLinearColor(0.0f,0.4f,0.75f)); break;
	case EItemRarityData::Legendary: ItemBorder->SetBrushColor(FLinearColor(1.0f,0.45f,0.0f)); break;
	default: ItemBorder->SetBrushColor(FLinearColor::White); break;
	}

	// --- Set icon ---
	ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);

	// --- Tampilkan jumlah kalau stackable ---
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

	if (SlotData.IsEmpty() || !ItemRow) return;

	if (!DragItemVisualClass) return;

	UDragItemVisual* DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
	DragVisual->ItemIcon->SetBrushFromTexture(ItemRow->ItemAsset.Icon);
	DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

	if (ItemRow->ItemNumeric.IsStackable())
		DragVisual->ItemQuantity->SetText(FText::AsNumber(SlotData.Quantity));
	else
		DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DragOp = NewObject<UItemDragDropOperation>();
	DragOp->SourceInventory = nullptr; // nanti bisa set dari InventoryWidget saat buat slot
	DragOp->SlotData = SlotData;
	DragOp->DefaultDragVisual = DragVisual;
	DragOp->Pivot = EDragPivot::CenterCenter;

	OutOperation = DragOp;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}