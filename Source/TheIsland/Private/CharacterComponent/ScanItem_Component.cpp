#include "CharacterComponent/ScanItem_Component.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Inventory/Item/ItemOrigin.h"
#include "Inventory/DataStruct/BaseItem.h"
#include "Inventory/DataStruct/FoodItem.h"

UScanItem_Component::UScanItem_Component()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Hanya create default subobject
    ScanCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ScanCollider"));
    ScanCollider->InitSphereRadius(75.f);
    ScanCollider->SetRelativeLocation(FVector(80.f, 0.f, -60.f));
    ScanCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ScanCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    ScanCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ScanCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    ScanCollider->SetGenerateOverlapEvents(true);
}

void UScanItem_Component::BeginPlay()
{
    Super::BeginPlay();

    if (ScanCollider && GetOwner())
    {
        ScanCollider->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        ScanCollider->OnComponentBeginOverlap.AddDynamic(this, &UScanItem_Component::OnScanBeginOverlap);
        ScanCollider->OnComponentEndOverlap.AddDynamic(this, &UScanItem_Component::OnScanEndOverlap);
    }
}

void UScanItem_Component::OnScanBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    if (!ScannedActors.Contains(OtherActor))
    {
        ScannedActors.Add(OtherActor);
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
            FString::Printf(TEXT("Scan: %s"), *OtherActor->GetName()));
    }
}

void UScanItem_Component::OnScanEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    ScannedActors.Remove(OtherActor);

    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
        FString::Printf(TEXT("Out of range: %s"), *OtherActor->GetName()));
}

void UScanItem_Component::Interact()
{
    if (ScannedActors.Num() == 0) return;

    AActor* TargetActor = ScannedActors[0];
    if (!IsValid(TargetActor))
    {
        ScannedActors.RemoveAt(0);
        return;
    }

    if (AItemOrigin* ItemActor = Cast<AItemOrigin>(TargetActor))
    {
        if (ItemActor->ItemDataTable && ItemActor->ItemRowName != NAME_None)
        {
            static const FString Context(TEXT("Item Lookup"));
            if (ItemActor->ItemType == EItemTypeSub::Normal)
            {
                if (FBaseItem* BaseData = ItemActor->ItemDataTable->FindRow<FBaseItem>(ItemActor->ItemRowName, Context))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
                        FString::Printf(TEXT("Picked Normal Item: %s"), *BaseData->ItemID.ToString()));
                }
            }
            else if (ItemActor->ItemType == EItemTypeSub::Food)
            {
                if (FFoodItem* FoodData = ItemActor->ItemDataTable->FindRow<FFoodItem>(ItemActor->ItemRowName, Context))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
                        FString::Printf(TEXT("Picked Food: %s Heal: %d"), *FoodData->ItemID.ToString(), FoodData->HealAmount));
                }
            }
        }
    }

    ScannedActors.RemoveAt(0);

    if (IsValid(TargetActor) && !TargetActor->IsPendingKillPending())
    {
        TargetActor->Destroy();
    }
}
