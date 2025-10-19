#include "Inventory/Item/ItemOrigin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/DataStruct/BaseItem.h"
#include "Inventory/DataStruct/FoodItem.h"

AItemOrigin::AItemOrigin()
{
	PrimaryActorTick.bCanEverTick = false;

	// Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Collision
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(MeshComponent);
	CollisionComponent->SetSphereRadius(100.f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AItemOrigin::BeginPlay()
{
	Super::BeginPlay();

	if (ItemDataTable && ItemRowName != NAME_None)
	{
		if (ItemType == EItemTypeSub::Normal)
		{
			if (FBaseItem* BaseData = ItemDataTable->FindRow<FBaseItem>(ItemRowName, TEXT("")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Normal Item: %s"), *BaseData->ID.ToString());
			}
		}
		else if (ItemType == EItemTypeSub::Food)
		{
			if (FFoodItem* FoodData = ItemDataTable->FindRow<FFoodItem>(ItemRowName, TEXT("")))
			{
				UE_LOG(LogTemp, Warning, TEXT("Food: %s Heal: %d"), 
					*FoodData->ID.ToString(), FoodData->HealAmount);
			}
		}
	}
}

void AItemOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
