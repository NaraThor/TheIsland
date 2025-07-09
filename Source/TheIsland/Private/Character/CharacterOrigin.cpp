// Fill out your copyright notice in the Description page of Project Settings.

#include "TheIsland/Public/Character/CharacterOrigin.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
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

	// Collision Iteract
	CollisionInteract = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionInteract"));
	CollisionInteract->SetupAttachment(GetCapsuleComponent());
	CollisionInteract->InitSphereRadius(75.f);
	CollisionInteract->SetRelativeLocation(FVector(80.f, 0.f, -60.f));
	
	CollisionInteract->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionInteract->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionInteract->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionInteract->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	// Aktifkan overlap events
	CollisionInteract->SetGenerateOverlapEvents(true);
	
}

// Called when the game starts or when spawned
void ACharacterOrigin::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionInteract)
	{
		CollisionInteract->OnComponentBeginOverlap.AddDynamic(this, &ACharacterOrigin::OnScanOverlap);
	}

	
}

// Called every frame
void ACharacterOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActorsScannedThisFrame.Empty(); // Reset setiap frame
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
	GEngine-> AddOnScreenDebugMessage(-1,5.0f,FColor::Red,"Test Input");
	
}

void ACharacterOrigin::OnScanOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!OtherActor || OtherActor == this) return;
	//FString DetectedName = OtherActor->GetName();
	//if (OtherActor->IsA(APawn::StaticClass()))
	//{
		//if (OtherComp != OtherActor->GetRootComponent()) return;
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
	//		FString::Printf(TEXT("Pawn Terdeteksi: %s"), *DetectedName));
	//}
	//else
	//{
		//if (OtherComp != OtherActor->GetRootComponent()) return;
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
	//		FString::Printf(TEXT("Actor Terdeteksi: %s"), *DetectedName));
	//}
	


	////////////////////////
	if (!OtherActor || OtherActor == this) return;

	// Cegah deteksi ganda
	if (ActorsScannedThisFrame.Contains(OtherActor)) return;
	ActorsScannedThisFrame.Add(OtherActor);

	// Sekarang ini pasti hanya sekali per actor per frame
	if (OtherActor->IsA(APawn::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
			FString::Printf(TEXT("Pawn Terdeteksi: %s"), *OtherActor->GetName()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
			FString::Printf(TEXT("Actor Terdeteksi: %s"), *OtherActor->GetName()));
	}
}




