#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/DataStruct/ItemDataStruct.h"
#include "ItemOrigin.generated.h"

class USphereComponent;

UCLASS()
class THEISLAND_API AItemOrigin : public AActor
{
	GENERATED_BODY()

public:	
	AItemOrigin();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Komponen visual
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// Komponen Collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	// Data Asset item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UItemDataStruct* ItemData;
};
