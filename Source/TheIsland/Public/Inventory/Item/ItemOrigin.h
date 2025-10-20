#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Inventory/DataStruct/BaseItem.h"
#include "ItemOrigin.generated.h"

class USphereComponent;

/*
UENUM(BlueprintType)
enum class EItemTypeSub : uint8
{
	Normal,
	Food
};
*/

UCLASS()
class THEISLAND_API AItemOrigin : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AItemOrigin();
	
	// Komponen visual
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// Komponen collision (untuk overlap pickup)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	// ------------------- ITEM DATA -------------------

	// Dropdown otomatis untuk pilih item dari DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Item Initialization")
	FDataTableRowHandle ItemRowHandle;

	// Jumlah item (default 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity = 1;

	// Cache data dari DataTable agar bisa dibaca di runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Cached Data")
	FName CachedItemID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Cached Data")
	FText CachedItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Cached Data")
	EItemTypeData CachedItemType;
	virtual void Tick(float DeltaTime) override;

	// ------------------- FUNCTION -------------------
	UFUNCTION(CallInEditor, BlueprintCallable, Category="Pickup | Item Initialization")
	void InitializePickup();
	
	//Question
	//void InitializePickup();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
