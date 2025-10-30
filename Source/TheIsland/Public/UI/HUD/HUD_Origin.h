#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Origin.generated.h"

class UMainMenu;
class UInventory_Component;
class UInventoryWidget;

UCLASS()
class THEISLAND_API AHUD_Origin : public AHUD
{
	GENERATED_BODY()

public:

	///////////// VARIABLE ///////////
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf <UMainMenu> MainMenuClass;
/*
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf <UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf <UUserWidget> CrosshairWidgetClass;
*/	
	bool bIsMenuVisible;

	///////////// FUNCTION ///////////
	AHUD_Origin();
	
	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();
/*
	void ShowCrosshair();
	void HideCrosshair();
	
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;
*/	

protected:
	
	UPROPERTY()
	UMainMenu* MainMenuWidget;
	
/*
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	*/

	///////////// FUNGTION ///////////
	virtual void BeginPlay() override;
};
