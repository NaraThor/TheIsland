#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Origin.generated.h"

class UInventory_Component;
class UInventoryWidget;

UCLASS()
class THEISLAND_API AHUD_Origin : public AHUD
{
	GENERATED_BODY()

public:
	AHUD_Origin();

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	UInventory_Component* CachedInventory;
	
public:
	
	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION()
	void OnInventoryUpdatedHandler();
};
