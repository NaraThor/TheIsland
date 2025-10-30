#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class ACharacterOrigin;

UCLASS()
class THEISLAND_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	ACharacterOrigin* PlayerCharacter;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
};
