// InventorySlotWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UBaseItem;
class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference (UBaseItem* ItemIn) {ItemReference= ItemIn;};

	FORCEINLINE UBaseItem* GetItemReference () const {return ItemReference;}


protected:
	/*
	UPROPERTY(EditDefaultsOnly,Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly,Category="Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;
	*/
	
	UPROPERTY(VisibleAnywhere,Category="Inventory Slot")
	UBaseItem* ItemReference;
	UPROPERTY(VisibleAnywhere,Category="Inventory Slot",meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere,Category="Inventory Slot",meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere,Category="Inventory Slot",meta=(BindWidget))
	UTextBlock* ItemQuantity;
	

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//drag drop

	///Disini..

	
};
