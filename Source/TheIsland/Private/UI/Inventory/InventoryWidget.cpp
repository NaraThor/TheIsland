// InventoryWidget.cpp
#include "UI/Inventory/InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/InventorySlotWidget.h"

void UInventoryWidget::InitializeGrid(int32 Columns, int32 Rows)
{
	if (!GridPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridPanel is NULL!"));
		return;
	}

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotWidgetClass is NULL!"));
		return;
	}

	GridPanel->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("InitializeGrid called: %d x %d"), Columns, Rows);

	for (int32 Row = 0; Row < Rows; Row++)
	{
		for (int32 Col = 0; Col < Columns; Col++)
		{
			UInventorySlotWidget* NewSlot = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
			if (NewSlot)
			{
				GridPanel->AddChildToUniformGrid(NewSlot, Row, Col);
				UE_LOG(LogTemp, Warning, TEXT("Slot added at Row=%d, Col=%d"), Row, Col);
			}
		}
	}
}