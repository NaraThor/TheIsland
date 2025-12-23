#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponent/Inventory_Component.h"
#include "InventorySlotWidget.generated.h"

class UDragItemVisual;
class UImage;
class UTextBlock;
class UInventoryWidget;
struct FDataItem;
struct FInventorySlot;

UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(
		int32 InIndex,const FInventorySlot& InSlot,const FDataItem* InRow,UInventoryWidget* InInventoryWidget);

	int32 GetSlotIndex() const { return SlotIndex; }

protected:
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;

private:
	void RefreshVisual();

	int32 SlotIndex = INDEX_NONE;
	FInventorySlot SlotData;
	const FDataItem* ItemRow = nullptr;

	UPROPERTY()
	UInventoryWidget* InventoryWidgetRef;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY(EditDefaultsOnly, Category="Drag")
	TSubclassOf<UDragItemVisual> DragVisualClass;
};
