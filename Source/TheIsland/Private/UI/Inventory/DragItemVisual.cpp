#include "UI/Inventory/DragItemVisual.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Data/InventoryTypes.h"
#include "Inventory/DataStruct/DataItem.h"

void UDragItemVisual::InitVisual(
	const FInventorySlot& InSlot,
	const FDataItem* InItemRow)
{
	if (!ItemIcon || !ItemQuantity) return;

	if (!InItemRow || InSlot.IsEmpty())
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantity->SetText(FText::GetEmpty());
		return;
	}

	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	ItemIcon->SetBrushFromTexture(InItemRow->ItemAsset.Icon);
	ItemQuantity->SetText(FText::AsNumber(InSlot.Quantity));
}
