#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Data/InventoryTypes.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
struct FDataItem;
class UDragItemVisual;
class UInventoryWidget;
class UDragDropOperation;


UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// =====================================================
	// INIT
	// =====================================================
	void InitSlot(
		int32 InIndex,
		const FInventorySlot& InSlot,
		const FDataItem* InRow,
		UInventoryWidget* InInventoryWidget);

	int32 GetSlotIndex() const { return SlotIndex; }

	// =====================================================
	// UI UPDATE
	// =====================================================
	void RefreshVisual();

protected:

	// =====================================================
	// UUserWidget OVERRIDE
	// =====================================================
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragCancelled(
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

protected:

	// =====================================================
	// BIND WIDGET
	// =====================================================
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity = nullptr;

	// =====================================================
	// DRAG VISUAL
	// =====================================================
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Drag")
	TSubclassOf<UDragItemVisual> DragVisualClass;

protected:

	// =====================================================
	// SLOT DATA
	// =====================================================
	int32 SlotIndex = INDEX_NONE;

	FInventorySlot SlotData;

	const FDataItem* ItemRow = nullptr;

	UPROPERTY()
	UInventoryWidget* InventoryWidgetRef = nullptr;

protected:

	// =====================================================
	// INPUT STATE (TEMP, PER DRAG)
	// =====================================================
	bool bShiftHeld    = false;   // Half split
	bool bRightAltHeld = false;   // Split 1
};