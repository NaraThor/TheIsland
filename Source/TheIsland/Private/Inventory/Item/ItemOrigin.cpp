#include "Inventory/Item/ItemOrigin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

AItemOrigin::AItemOrigin()
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void AItemOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
