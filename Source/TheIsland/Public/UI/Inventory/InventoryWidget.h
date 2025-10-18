// InventoryWidget.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventory_Component;
class UHorizontalBox;
class UInventorySlotWidget;

UCLASS()
class THEISLAND_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	// 🔹 Panggil ini untuk menampilkan data inventory sebenarnya
	UFUNCTION(BlueprintCallable)
	void RefreshInventory(class UInventory_Component* InventoryRef);
	
};
