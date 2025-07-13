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
#include "Engine/Engine.h"
#include "Components/InputComponent.h"

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
	if (ScanComponent)
	{
		ScanComponent->Interact();
	}
}

void ACharacterOrigin::TestInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Test Input Pressed"));
	if (ScanComponent)
	{
		ScanComponent->Interact(); // atau aksi lain
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,FString::Printf(TEXT("Test Work...")));
}
