#include "UI/HUD/UIHandler/MainMenu.h"
#include "Character/CharacterOrigin.h"
#include "Handler/ItemDragDropOperation.h"
#include "UI/Inventory/InventoryWidget.h"


void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ACharacterOrigin>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (!ItemDragDrop)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error 1"));
		return false;
	}
	

	if (ItemDragDrop->ItemID == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error 2"));
		return false;
	}
	
	UE_LOG(LogTemp, Warning,
		TEXT("Item %s Drop Out"),
		*ItemDragDrop->ItemID.ToString());

	return true;  // penting supaya event tidak bubble!!
}


