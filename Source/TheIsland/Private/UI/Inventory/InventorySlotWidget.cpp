// InventorySlotWidget.cpp
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Item/BaseItem.h"

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

		default: ;
		}
		
		ItemIcon -> SetBrushFromTexture(ItemReference->ItemAsset.Icon);

		if (ItemReference-> ItemNumeric.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}



/*
if (QuantityText)
	{
		if (SlotData.Quantity > 0)
			QuantityText->SetText(FText::AsNumber(SlotData.Quantity));
		else
			QuantityText->SetText(FText::GetEmpty());
	}

	// TODO: Set ItemIcon pakai DataTable/DataAsset
 */

