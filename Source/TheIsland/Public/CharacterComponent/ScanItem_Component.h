#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScanItem_Component.generated.h"

class USphereComponent;
class AItemOrigin;
class IInteractionInterface;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UScanItem_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UScanItem_Component();

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


	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	void ClearScan();
	void HighlightInteractable(AActor* NewTarget);
	void ProccessInteractable(TArray<AActor*> ScannedActor);


public:
	/** Ambil target pertama yang ter-scan */
	AActor* GetCurrentTarget() const;


};
