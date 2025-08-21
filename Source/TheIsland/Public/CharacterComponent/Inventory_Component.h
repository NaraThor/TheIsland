// Inventory_Component.h
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/DataStruct/InventorySlot.h"
#include "Inventory_Component.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UInventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory_Component();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
	TArray<FInventorySlot> Slots;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeInventory(int32 Size);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(FName ItemID, int32 Quantity);
};
