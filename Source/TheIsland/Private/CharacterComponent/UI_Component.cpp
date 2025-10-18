// UI_Component.cpp
#include "CharacterComponent/UI_Component.h"
#include "UI/Inventory/InventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

UUI_Component::UUI_Component()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsInventoryVisible = false;
}

void UUI_Component::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryWidgetClass)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			InventoryWidgetInstance = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
			if (InventoryWidgetInstance)
			{
				InventoryWidgetInstance->AddToViewport();
				InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);

				// Mouse tetap tersembunyi & input tetap ke game
				PC->bShowMouseCursor = false;
				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);
			}
		}
	}
}

void UUI_Component::ShowInventory()
{
}
