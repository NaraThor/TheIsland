#include "UI/HUD/HUD_Origin.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponent/Inventory_Component.h"
#include "GameFramework/Character.h"
#include "UI/Inventory/InventoryWidget.h"
#include "TimerManager.h"

AHUD_Origin::AHUD_Origin()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHUD_Origin::BeginPlay()
{
	Super::BeginPlay();

	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD_Origin: InventoryWidgetClass not set!"));
		return;
	}

	// 🔹 Buat dan tampilkan widget
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("HUD_Origin: Failed to create InventoryWidget."));
		return;
	}
	InventoryWidget->AddToViewport();

	// 🔹 Ambil karakter & inventory
	ACharacter* PlayerChar = Cast<ACharacter>(GetOwningPawn());
	if (!PlayerChar)
	{
		PlayerChar = GetWorld()->GetFirstPlayerController()->GetCharacter();
	}

	CachedInventory = PlayerChar ? PlayerChar->FindComponentByClass<UInventory_Component>() : nullptr;
	if (!CachedInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD_Origin: No Inventory_Component found."));
		return;
	}

	// 🔹 Hubungkan delegate
	CachedInventory->OnInventoryUpdated.AddDynamic(this, &AHUD_Origin::OnInventoryUpdatedHandler);

	// 🔹 Lakukan refresh awal
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (InventoryWidget && CachedInventory)
		{
			InventoryWidget->RefreshInventory(CachedInventory);
			UE_LOG(LogTemp, Log, TEXT("HUD: Initial inventory refresh done (%d slots)."), CachedInventory->Slots.Num());
		}
	});
}

void AHUD_Origin::OnInventoryUpdatedHandler()
{
	if (InventoryWidget && CachedInventory)
	{
		InventoryWidget->RefreshInventory(CachedInventory);
		UE_LOG(LogTemp, Log, TEXT("HUD: Inventory auto-refreshed via delegate!"));
	}
}
