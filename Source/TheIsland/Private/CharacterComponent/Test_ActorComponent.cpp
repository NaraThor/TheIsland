	// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/Test_ActorComponent.h"

// Sets default values for this component's properties
UTest_ActorComponent::UTest_ActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

	// Called when the game starts
void UTest_ActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTest_ActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTest_ActorComponent::DoSomething()
{

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Tombol E ditekan oleh Komponen!"));
	
}



