#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI_Component.generated.h"

extern UDataTable* GItemDataTable;

class UInventoryWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UUI_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UUI_Component();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UInventoryWidget* InventoryWidgetInstance;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	bool bIsInventoryVisible;

public:
	//	UFUNCTION(BlueprintCallable)
	//	void ToggleInventory();

	UFUNCTION(BlueprintCallable)
	void ShowInventory();

	//	UFUNCTION(BlueprintCallable)
	//	void HideInventory();
};
