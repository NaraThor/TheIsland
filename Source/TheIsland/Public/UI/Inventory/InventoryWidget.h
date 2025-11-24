// InventoryWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Handler/ItemDragDropOperation.h"
#include "InventoryWidget.generated.h"

class ACharacterOrigin;
class UWrapBox;
class UInventory_Component;
class UInventorySlotWidget;

UCLASS()
class THEISLAND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void RefreshInventory();

	void HandleSlotDrop(UInventorySlotWidget* DropTargetSlot,UItemDragDropOperation* DragOp);

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY()
	ACharacterOrigin* PlayerCharacter;

	UPROPERTY()
	UInventory_Component* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;

protected:
	//virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//virtual bool NativeOnDragOver(const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation) override;
	
};
