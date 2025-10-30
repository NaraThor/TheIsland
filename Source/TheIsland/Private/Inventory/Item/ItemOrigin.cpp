#include "Inventory/Item/ItemOrigin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Inventory/DataStruct/DataItem.h"

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

	InitializePickup();
	
}

void AItemOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemOrigin::InitializePickup()
{
	if (!ItemRowHandle.DataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InitializePickup] DataTable belum diassign."));
		return;
	}

	if (ItemRowHandle.RowName == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InitializePickup] RowName belum diatur."));
		return;
	}

	// Ambil data berdasarkan RowHandle
	const FDataItem* BaseItem = ItemRowHandle.GetRow<FDataItem>(TEXT("InitializePickup"));
	if (!BaseItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InitializePickup] Gagal menemukan row %s di DataTable."), *ItemRowHandle.RowName.ToString());
		return;
	}

	// Cache data penting
	CachedItemID = BaseItem->ID;
	CachedItemName = BaseItem->ItemText.Name;
	CachedItemType = BaseItem->ItemType;

	// Set mesh dari data item
	if (BaseItem->ItemAsset.Mesh)
	{
		MeshComponent->SetStaticMesh(BaseItem->ItemAsset.Mesh);
	}
	else
	{
		MeshComponent->SetStaticMesh(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("[InitializePickup] Item %s tidak memiliki mesh."), *CachedItemID.ToString());
	}

	// Log debug friendly
	UE_LOG(LogTemp, Log, TEXT("[InitializePickup] Loaded item: %s (%s) | Type: %s | Stackable: %s"),
		*CachedItemName.ToString(),
		*CachedItemID.ToString(),
		*UEnum::GetValueAsString(CachedItemType),
		BaseItem->ItemNumeric.bIsStackable ? TEXT("Yes") : TEXT("No")
	);
}

#if WITH_EDITOR
void AItemOrigin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Jika properti ItemRowHandle berubah, langsung update data
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AItemOrigin, ItemRowHandle))
	{
		InitializePickup();
	}
}
#endif