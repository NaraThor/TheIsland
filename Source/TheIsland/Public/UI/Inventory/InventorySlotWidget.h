#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponent/Inventory_Component.h"// untuk FInventorySlot & FInventoryItemRow
#include "InventorySlotWidget.generated.h"

class UBorder;
class UImage;
class UTextBlock;
class UDragItemVisual;

UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Set data slot + row
	void SetItemData(const FInventorySlot& Slot, const FDataItem* Row);

protected:

	// --- UI Widgets ---
	UPROPERTY(meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	// --- Slot Data ---
	FInventorySlot SlotData;
	const FDataItem* ItemRow;

	// --- Overrides ---
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;
};
