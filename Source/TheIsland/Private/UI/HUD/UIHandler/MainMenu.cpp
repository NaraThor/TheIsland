#include "UI/HUD/UIHandler/MainMenu.h"
#include "Character/CharacterOrigin.h"
#include "Handler/ItemDragDropOperation.h"
#include "UI/Inventory/InventoryWidget.h"
#include "CharacterComponent/Inventory_Component.h"


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

	ACharacterOrigin* Player = Cast<ACharacterOrigin>(GetOwningPlayerPawn());
	if (!Player) return true;

	UInventory_Component* Inv = Player->GetInventory();   // << BENAR
	if (!Inv) return true;

	// Drop berdasarkan slot index
	Inv->DropItemBySlotIndex(ItemDragDrop->SlotIndex, ItemDragDrop->DragQuantity);

	UE_LOG(LogTemp, Warning, TEXT("Dropped %s x%d"),
		*ItemDragDrop->ItemID.ToString(),
		ItemDragDrop->DragQuantity);

	return true;
}


