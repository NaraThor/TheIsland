// InventorySlotWidget.cpp
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Handler/ItemDragDropOperation.h"
#include "Inventory/Item/BaseItem.h"
#include "UI/Inventory/DragItemVisual.h"

//DragDrop
void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
		switch (ItemReference -> ItemRarity)
		{
		case EItemRarityData::Common:
			ItemBorder->SetBrushColor(FLinearColor(FLinearColor::Gray));
			break;
		case EItemRarityData::Rare:
			ItemBorder->SetBrushColor(FLinearColor(FLinearColor::White));
			break;
		case EItemRarityData::Epic:
			ItemBorder->SetBrushColor(FLinearColor(0.0f,0.71f,0.169f));
			break;
		case EItemRarityData::Mythic:
			ItemBorder->SetBrushColor(FLinearColor(0.0f,0.4f,0.75f));
			break;
		case EItemRarityData::Legendary:
			ItemBorder->SetBrushColor(FLinearColor(1.0f,0.45f,0.0f)); //Orange 
			break;

		default:
			break;
		}
		
		ItemIcon -> SetBrushFromTexture(ItemReference->ItemAsset.Icon);

		// Tampilkan jumlah hanya kalau stackable
		if (ItemReference-> ItemNumeric.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		// Slot kosong: hapus icon dan sembunyikan text quantity
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		// (opsional) ubah warna border jadi abu-abu atau transparan
		ItemBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.2f));
		
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(
	const FGeometry& InGeometry,const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ItemReference)
	{
		return Reply.Handled().DetectDrag(TakeWidget(),EKeys::LeftMouseButton);
	}
	//submenu on right click will happpen here
	return Reply.Unhandled();
}

void UInventorySlotWidget::NativeOnMouseLeave(
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventorySlotWidget::NativeOnDragDetected(
	const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent,OutOperation);

	// Cegah drag slot kosong
	if (!ItemReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to drag empty slot — ignored."));
		return;
	}
	
	if (DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual=CreateWidget<UDragItemVisual>(this,DragItemVisualClass);
		DragVisual -> ItemIcon->SetBrushFromTexture(ItemReference->ItemAsset.Icon);
		DragVisual -> ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemReference->ItemNumeric.bIsStackable
			? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity))
			: DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		UItemDragDropOperation* DragItemOperation =	NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;

		//Pivot Drag Location
		DragItemOperation-> Pivot = EDragPivot::CenterCenter;

		OutOperation = DragItemOperation;
	}
}

bool UInventorySlotWidget::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent,InOperation);
}

