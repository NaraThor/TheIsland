// CharacterOrigin.cpp

#include "Character/CharacterOrigin.h"
#include "CharacterComponent/ScanItem_Component.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "CharacterComponent/Inventory_Component.h"
#include "CharacterComponent/UI_Component.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include "Inventory/Item/ItemOrigin.h"

ACharacterOrigin::ACharacterOrigin()
{
	PrimaryActorTick.bCanEverTick = true;

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Scan Component
	ScanComponent = CreateDefaultSubobject<UScanItem_Component>(TEXT("ScanComponent"));

	// Inventory Component
	InventoryComponent= CreateDefaultSubobject<UInventory_Component>(TEXT("InventoryComponent"));
	
	// UI Component
	UIComponent= CreateDefaultSubobject<UUI_Component>(TEXT("UIComponent"));
}

void ACharacterOrigin::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterOrigin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup Enhanced Input
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_CharacterMovement, ETriggerEvent::Triggered, this, &ACharacterOrigin::Character_Movement);
		Input->BindAction(IA_CharacterLook, ETriggerEvent::Triggered, this, &ACharacterOrigin::Character_Look);
		Input->BindAction(IA_CharacterJump, ETriggerEvent::Triggered, this, &ACharacterOrigin::Character_Jump);
		Input->BindAction(IA_CharacterInteract, ETriggerEvent::Started, this, &ACharacterOrigin::InteractScan);
		Input->BindAction(IA_TestAction, ETriggerEvent::Started, this, &ACharacterOrigin::TestInput);
	}
}

void ACharacterOrigin::Character_Movement(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, InputVector.Y);
		AddMovementInput(RightDir, InputVector.X);
	}
}

void ACharacterOrigin::Character_Look(const FInputActionValue& InputValue)
{
	FVector2D LookAxis = InputValue.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void ACharacterOrigin::Character_Jump()
{
	Jump();
}

void ACharacterOrigin::InteractScan(const FInputActionValue& InputValue)
{
	if (!ScanComponent || !InventoryComponent) return;

	AActor* TargetActor = ScanComponent->GetCurrentTarget();
	if (!TargetActor) return;

	// Kalau target adalah item
	if (AItemOrigin* ItemActor = Cast<AItemOrigin>(TargetActor))
	{
		InventoryComponent->AddItem(ItemActor->ItemRowHandle.RowName, ItemActor->Quantity);

		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,
			FString::Printf(TEXT("Picked up: %s"), *ItemActor->ItemRowHandle.RowName.ToString()));

		ItemActor->Destroy();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan,
			FString::Printf(TEXT("Interacted with: %s"), *TargetActor->GetName()));
	}
}

void ACharacterOrigin::TestInput()
{
	//this is decoy
	
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,FString::Printf(TEXT("Test Work...")));
}
