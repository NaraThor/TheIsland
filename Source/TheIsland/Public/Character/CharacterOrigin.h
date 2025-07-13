// CharacterOrigin.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterOrigin.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UScanItem_Component;

UCLASS()
class THEISLAND_API ACharacterOrigin : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterOrigin();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Components")
	UScanItem_Component* ScanComponent;

	// Enhanced Input
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_CharacterMovement;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_CharacterJump;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_CharacterLook;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_CharacterInteract;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_TestAction;

	// Movement
	void Character_Movement(const FInputActionValue& InputValue);
	void Character_Look(const FInputActionValue& InputValue);
	void Character_Jump();

	// Interaction
	void InteractScan(const FInputActionValue& InputValue);
	void TestInput(); // Tombol test input

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
