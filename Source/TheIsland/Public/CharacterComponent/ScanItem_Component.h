#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScanItem_Component.generated.h"

class USphereComponent;
class AItemOrigin;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UScanItem_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UScanItem_Component();
	void Interact();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Scan")
	USphereComponent* ScanCollider;

	UPROPERTY()
	TArray<AActor*> ScannedActors;

	UFUNCTION()
	void OnScanBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void OnScanEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
