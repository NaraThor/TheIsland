// Fill out your copyright notice in the Description page of Project Settings.

#include "TheIsland/Public/Character/CharacterOrigin.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACharacterOrigin::ACharacterOrigin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraPlayerSpring"));
	SpringArm->SetupAttachment(RootComponent);
	// Mengatur kamera agar mengikuti rotasi pawn controller
	SpringArm->bUsePawnControlRotation = true;

	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	// Menyambungkan kamera ke RootComponent
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement=false;
	
}

// Called when the game starts or when spawned
void ACharacterOrigin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterOrigin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Add InputMapping context
	if (APlayerController* PlayerController = Cast <APlayerController>(Controller))
	{
		//get local player SubSystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
		//AddInputContext
			Subsystem->AddMappingContext(InputMapping, 0);
			
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		///IsTest
		Input->BindAction(IA_TestAction,ETriggerEvent::Triggered,this,&ACharacterOrigin::TestInput);
		///IsTest

		Input->BindAction(IA_CharacterMovement,ETriggerEvent::Triggered,this,&ACharacterOrigin::Character_Movement);
		Input->BindAction(IA_CharacterJump,ETriggerEvent::Triggered,this,&ACharacterOrigin::Character_Jump);
		Input->BindAction(IA_CharacterLook,ETriggerEvent::Triggered,this,&ACharacterOrigin::Character_Look);
		
	}
	
}


///Testo Border
void ACharacterOrigin::TestInput()
{

	GEngine-> AddOnScreenDebugMessage(-1,5.0f,FColor::Red,"Test Input");
}
///Testo Border


void ACharacterOrigin::Character_Movement(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		//GetForwardDirection
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);

		const FVector ForwardDiretion =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDiretion =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//AddMovementInput
		AddMovementInput(ForwardDiretion,InputVector.Y);
		AddMovementInput(RightDiretion,InputVector.X);
		
	}
}

void ACharacterOrigin::Character_Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
	
}

void ACharacterOrigin::Character_Jump()
{
	ACharacter::Jump();
	
}



