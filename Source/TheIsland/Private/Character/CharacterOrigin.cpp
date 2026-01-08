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
#include "Handler/InteractionInterface.h"
#include "Inventory/Item/ItemOrigin.h"
#include "UI/HUD/HUD_Origin.h"

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
	PlayerInventory= CreateDefaultSubobject<UInventory_Component>(TEXT("InventoryComponent"));
	
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

		// Inventory
		Input->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ACharacterOrigin::ToggleMenu);
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

////////////////////////////////////// Custom Fungtion

void ACharacterOrigin::InteractScan(const FInputActionValue& InputValue)
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);

		// UE_LOG(LogTemp,Warning,TEXT("Interactable Work {Character}"));
	}
}

void ACharacterOrigin::TestInput()
{
	//this is decoy
	
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow,FString::Printf(TEXT("Test Work...")));
}

void ACharacterOrigin::BeginInteract()
{
}

void ACharacterOrigin::EndInteract()
{
}

void ACharacterOrigin::Interact()
{
	//check this 
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable -> Interact(this);

		//UE_LOG(LogTemp,Warning,TEXT("Interactable Work {Character}"));
	}
}

void ACharacterOrigin::ToggleMenu()
{
	if (HUD)
	{
		HUD->ToggleMenu();

		if (HUD->bIsMenuVisible)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Trigger Active.."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Hidden.."));
		}
	}
	else
	{
		// Kalau HUD belum ada (null), tampilkan warning agar tahu penyebabnya
		UE_LOG(LogTemp, Error, TEXT("ToggleMenu() failed: HUD is nullptr!"));
		
		// Kamu bisa tambahkan mekanisme fallback opsional di sini, misal:
		// 1. Coba cari ulang HUD dari PlayerController
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			AHUD* FoundHUD = PC->GetHUD();
			if (FoundHUD)
			{
				HUD = Cast<AHUD_Origin>(FoundHUD);
				if (HUD)
				{
					UE_LOG(LogTemp, Warning, TEXT("HUD reference restored successfully."));
					HUD->ToggleMenu();
					return;
				}
			}
		}

		// 2. Kalau masih gagal, beri tahu lewat log
		UE_LOG(LogTemp, Error, TEXT("Failed to recover HUD reference."));
	}
}

void ACharacterOrigin::DropItem(int32 SlotIndex,int32 Quantity)
{
	if (!PlayerInventory)
		return;

	FVector DropLocation =
		GetActorLocation() +
		GetActorForwardVector() * 120.f;

	PlayerInventory->DropItemToWorld(
		SlotIndex,
		Quantity,
		DropLocation,
		GetActorRotation()
	);
}
