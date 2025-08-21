// InventoryWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/DataStruct/InventorySlot.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventorySlotWidget;

UCLASS()
class THEISLAND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* GridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UFUNCTION(BlueprintCallable)
	void InitializeGrid(int32 Columns, int32 Rows);
};
