#include "UI/HUD/HUD_Origin.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponent/Inventory_Component.h"
#include "GameFramework/Character.h"
#include "UI/Inventory/InventoryWidget.h"
#include "TimerManager.h"
#include "UI/HUD/UIHandler/MainMenu.h"


AHUD_Origin::AHUD_Origin()
{
	
}

void AHUD_Origin::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}


	// if UI lainya..
}


void AHUD_Origin::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget -> SetVisibility(ESlateVisibility::Visible);
		
	}
}

void AHUD_Origin::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget -> SetVisibility(ESlateVisibility::Collapsed);
		
	}
}

void AHUD_Origin::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController() -> SetInputMode(InputMode);
		GetOwningPlayerController() -> SetShowMouseCursor(false);
		
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController() -> SetInputMode(InputMode);
		GetOwningPlayerController() -> SetShowMouseCursor(true);
		
	}
}

/*
void AHUD_Origin::ShowCrosshair()
{
}

void AHUD_Origin::HideCrosshair()
{
}


void AHUD_Origin::ShowInteractionWidget() const
{
}

void AHUD_Origin::HideInteractionWidget() const
{
}

void AHUD_Origin::UpdateInteractionWidget(
	const FInteractableData* InteractableData)
const
{
}
*/
