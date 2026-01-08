#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Handler/InteractionInterface.h"
#include "ItemOrigin.generated.h"

class UStaticMeshComponent;
struct FDataItem;
class UDataTable;
class ACharacterOrigin;

UCLASS()
class THEISLAND_API AItemOrigin : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AItemOrigin();

	/** Dipanggil saat item spawn natural (pickup di world) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializePickup();

	/** Dipanggil saat item di-drop dari inventory */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void InitializeDrop(FName InItemID, int32 InQuantity);

	
	FORCEINLINE FName GetItemID() const { return ItemID; }
	FORCEINLINE int32 GetQuantity() const { return Quantity; }

	void SetItemDataTable(UDataTable* InDataTable);

protected:
	virtual void BeginPlay() override;

	// Interaction Interface
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ACharacterOrigin* PlayerCharacter) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Item|Component")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Item|Data")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(VisibleInstanceOnly, Category = "Item|Runtime")
	FName ItemID;

	UPROPERTY(EditAnywhere, Category = "Item|Runtime")
	int32 Quantity = 1;

	void UpdateVisualFromData(const FDataItem* ItemData);
	void TakePickup(ACharacterOrigin* Taker);

	UPROPERTY(VisibleInstanceOnly, Category="Item|Runtime")
	bool bIsDroppedItem = false;

	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
