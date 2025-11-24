#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponent/Inventory_Component.h"// untuk FInventorySlot & FInventoryItemRow
#include "InventorySlotWidget.generated.h"

class UBorder;
class UImage;
class UTextBlock;
class UDragItemVisual;
struct FDataItem;

UCLASS()
class THEISLAND_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// index slot ini
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	int32 SlotIndex = -1;
	
	// Data terdalam slot
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	FInventorySlot SlotData;

	UPROPERTY()
	UInventory_Component* OwningInventory;

	// Set data slot + row
	void SetItemData(const FInventorySlot& InSlot, const FDataItem* Row);

	// Set index
	UFUNCTION()
	void SetSlotIndex(int32 InIndex) { SlotIndex = InIndex; }
	
	void RefreshVisual();

	// =====================================================
	// DELEGATE AGAR PARENT BISA TERIMA EVENT DROP
	// =====================================================
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnSlotDroppedSignature,UInventorySlotWidget*, TargetSlot,UItemDragDropOperation*, DragOp);
	
	//UPROPERTY(BlueprintAssignable, Category = "Inventory")
	//FOnSlotDroppedSignature OnSlotDropped;

	UPROPERTY()
	class UInventoryWidget* InventoryWidgetRef;

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
	const FDataItem* ItemRow;


	
	// --- Overrides ---
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;
};
