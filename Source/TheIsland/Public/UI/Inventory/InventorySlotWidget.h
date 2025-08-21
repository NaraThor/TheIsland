// InventorySlotWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/DataStruct/InventorySlot.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuantityText;

	UFUNCTION(BlueprintCallable)
	void UpdateSlot(const FInventorySlot& SlotData);
};
