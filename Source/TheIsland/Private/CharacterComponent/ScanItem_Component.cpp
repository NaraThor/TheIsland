// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/ScanItem_Component.h"

#include "Components/SphereComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UScanItem_Component::UScanItem_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Membuat Sphere Component untuk scan
	ScanCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ScanCollider"));
	if (ScanCollider)
	{
		ScanCollider->InitSphereRadius(75.f);
		ScanCollider->SetRelativeLocation(FVector(80.f, 0.f, -60.f));
		ScanCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ScanCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
		ScanCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		ScanCollider->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		ScanCollider->SetGenerateOverlapEvents(true);
		//JANGAN setup attachment di sini, karena ini bukan scene component
	}
}

// Called when the game starts
void UScanItem_Component::BeginPlay()
{
	Super::BeginPlay();

	if (ScanCollider)
	{
		// Pastikan attach ke root milik actor (misalnya ke capsule karakter)
		if (AActor* Owner = GetOwner())
		{
			if (USceneComponent* Root = Owner->GetRootComponent())
			{
				ScanCollider->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);

				// Tambahkan fungsi overlap
				ScanCollider->OnComponentBeginOverlap.AddDynamic(this, &UScanItem_Component::OnOverlapBegin);
				ScanCollider->OnComponentEndOverlap.AddDynamic(this, &UScanItem_Component::OnOverlapEnd);
			}
		}
	}
}

// Fungsi Interact
void UScanItem_Component::Interact()
{
	if (ScannedActors.Num() > 0)
	{
		AActor* Target = ScannedActors[0];
		if (IsValid(Target))
		{
			ScannedActors.RemoveAt(0);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
				FString::Printf(TEXT("Interact: %s"), *Target->GetName()));
			Target->Destroy();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Silver, TEXT("No actor to interact."));
	}
}

// Fungsi ketika objek masuk area scan
void UScanItem_Component::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
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

// Fungsi ketika objek keluar dari area scan
void UScanItem_Component::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	if (ScannedActors.Contains(OtherActor))
	{
		ScannedActors.Remove(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
			FString::Printf(TEXT("Out of range: %s"), *OtherActor->GetName()));
	}
}