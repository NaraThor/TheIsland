// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterOrigin.generated.h"

class USphereComponent;

UCLASS()
class THEISLAND_API ACharacterOrigin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterOrigin();
	

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Properti hanya dapat diakses oleh kelas ini atau turunannya

	// Kamera untuk karakter
	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	//SphereTraceComponent
	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionInteract;
	
	
	UPROPERTY()
	TSet<AActor*> ActorsScannedThisFrame;
	
	// Fungsi untuk bergerak maju/mundur
	//Movement
	//Jump
	//Crouch

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	//Movement
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* IA_CharacterMovement;

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* IA_CharacterJump;
	
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* IA_CharacterLook;



	/////Border
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	class UInputAction* IA_TestAction;
	void TestInput();
	//////Border



	
	void Character_Movement(const FInputActionValue& InputValue);
	void Character_Look(const FInputActionValue& InputValue);
	void Character_Jump();

	//This is Fungtion
	UFUNCTION()
	void OnScanOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	
};
