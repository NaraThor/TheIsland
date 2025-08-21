// InventorySlotWidget.cpp
#include "UI/Inventory/InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::UpdateSlot(const FInventorySlot& SlotData)
{
	if (QuantityText)
	{
		if (SlotData.Quantity > 0)
			QuantityText->SetText(FText::AsNumber(SlotData.Quantity));
		else
			QuantityText->SetText(FText::GetEmpty());
	}

	// TODO: Set ItemIcon pakai DataTable/DataAsset
}
