#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWrapBox;
class UInventory_Component;
class UInventorySlotWidget;
class ACharacterOrigin;
class UItemDragDropOperation;

UCLASS()
class THEISLAND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// === MARK: tambah referensi drag slot ===
	UPROPERTY()
	UInventorySlotWidget* DraggedSlot = nullptr;

	UFUNCTION()
	void RefreshInventory();

	void HandleSlotDrop(
		UInventorySlotWidget* TargetSlot,
		UItemDragDropOperation* DragOp);

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;

	void DropItemToWorld(int32 FromSlotIndex, int32 Quantity);
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	ACharacterOrigin* PlayerCharacter;

	UPROPERTY()
	UInventory_Component* InventoryRef;
};