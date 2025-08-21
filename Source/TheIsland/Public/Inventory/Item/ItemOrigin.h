#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemOrigin.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EItemTypeSub : uint8
{
	Normal,
	Food
};
UCLASS()
class THEISLAND_API AItemOrigin : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AItemOrigin();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	EItemTypeSub ItemType;

	// Row name di DataTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FName ItemRowName;

	// Komponen visual
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// Komponen Collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	// Data Asset item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 Quantity = 1;
	
	virtual void Tick(float DeltaTime) override;
};
