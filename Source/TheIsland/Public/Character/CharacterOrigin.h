// CharacterOrigin.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterOrigin.generated.h"

class AHUD_Origin;
class IInteractionInterface;
class UInventory_Component;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UScanItem_Component;
class UUI_Component;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
	
};


UCLASS()
class THEISLAND_API ACharacterOrigin : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterOrigin();

	FORCEINLINE UInventory_Component* GetInventory() const { return PlayerInventory; };


protected:

	UPROPERTY()
	AHUD_Origin* HUD;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UScanItem_Component* ScanComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UInventory_Component* PlayerInventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UUI_Component* UIComponent;

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

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Inventory;


	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;
	
	// Movement
	void Character_Movement(const FInputActionValue& InputValue);
	void Character_Look(const FInputActionValue& InputValue);
	void Character_Jump();

	// Interaction
	void InteractScan(const FInputActionValue& InputValue);
	void TestInput(); // Tombol test input

	void BeginInteract();
	void EndInteract();
	void Interact();

	void ToggleMenu();

	void DropItem(int32 SlotIndex,int32 Quantity);

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;
};
