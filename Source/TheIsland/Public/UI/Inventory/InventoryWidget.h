// InventoryWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class ACharacterOrigin;
class UWrapBox;
class UInventory_Component;
//class UHorizontalBox;
class UInventorySlotWidget;

UCLASS()
class THEISLAND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void RefreshInventory();

	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY()
	ACharacterOrigin* PlayerCharacter;

	UPROPERTY()
	UInventory_Component* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotClass;

protected:
	virtual void NativeOnInitialized() override;
	//virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;






	
	
	
	
	
	
	
	
	
	/*
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	// 🔹 Panggil ini untuk menampilkan data inventory sebenarnya
	UFUNCTION(BlueprintCallable)
	void RefreshInventory(class UInventory_Component* InventoryRef);
	*/
	
};
