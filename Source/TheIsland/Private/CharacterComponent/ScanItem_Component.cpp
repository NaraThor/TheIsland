#include "CharacterComponent/ScanItem_Component.h"

#include "Character/CharacterOrigin.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Handler/InteractionInterface.h"
#include "Inventory/Item/ItemOrigin.h"
#include "Inventory/DataStruct/DataItem.h"
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
        ProccessInteractable(ScannedActors);
        
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
            FString::Printf(TEXT("Scan: %s"), *OtherActor->GetName()));
    }
    
}

void UScanItem_Component::OnScanEndOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    ScannedActors.Remove(OtherActor);
    ProccessInteractable(ScannedActors);

    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
        FString::Printf(TEXT("Out of range: %s"), *OtherActor->GetName()));
}

AActor* UScanItem_Component::GetCurrentTarget() const
{
    AActor* Closest = nullptr;
    float ClosestDist = FLT_MAX;
    AActor* Owner = GetOwner();

    for (AActor* Target : ScannedActors)
    {
        if (!IsValid(Target)) continue;

        float Dist = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation());
        if (Dist < ClosestDist)
        {
            Closest = Target;
            ClosestDist = Dist;
        }
    }
    
    return Closest;
}

void UScanItem_Component::ClearScan()
{
    ScannedActors.RemoveAll([](AActor* Actor) {return !IsValid(Actor);});
}

void UScanItem_Component::HighlightInteractable(
    AActor* NewTarget)
{

    ///kalau mau hight light
    ///buttuh Item::BeginFocus() & Item::EndFocus()
    ///
    
}

void UScanItem_Component::ProccessInteractable(TArray<AActor*> ScannedActor)
{
    AActor* ClosestTarget = GetCurrentTarget();

    if (ClosestTarget && ClosestTarget->Implements<UInteractionInterface>())
    {
        ACharacterOrigin* OwnerCharacter = Cast<ACharacterOrigin>(GetOwner());
        if (OwnerCharacter)
        {
            // Set interface
            OwnerCharacter->TargetInteractable.SetObject(ClosestTarget);
            OwnerCharacter->TargetInteractable.SetInterface(
                Cast<IInteractionInterface>(ClosestTarget)
            );
        }
    }
}
