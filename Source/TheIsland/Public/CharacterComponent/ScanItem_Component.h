// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScanItem_Component.generated.h"

class USphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UScanItem_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UScanItem_Component();

	// Untuk dipanggil oleh karakter saat menekan tombol (misal tombol E)
	void Interact();

	// Getter agar komponen lain bisa akses collider-nya jika perlu
	USphereComponent* GetScanCollider() const { return ScanCollider; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Scan")
	USphereComponent* ScanCollider;

	UPROPERTY()
	TArray<AActor*> ScannedActors;

	// Event overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};