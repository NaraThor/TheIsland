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
	const FGeometry& InGeometry,
	const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* DragOp =
		Cast<UItemDragDropOperation>(InOperation);

	if (!DragOp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Drag Operation"));
		return false;
	}

	if (DragOp->ItemID == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemID"));
		return false;
	}

	if (DragOp->FromSlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid FromSlotIndex"));
		return false;
	}

	ACharacterOrigin* Player =
		Cast<ACharacterOrigin>(GetOwningPlayerPawn());
	if (!Player)
		return true;

	UInventory_Component* Inventory =
		Player->GetInventory();
	if (!Inventory)
		return true;

	Inventory->DropItemBySlotIndex(
		DragOp->FromSlotIndex,
		DragOp->DragQuantity
	);

	UE_LOG(LogTemp, Warning,
		TEXT("Dropped %s x%d from slot %d"),
		*DragOp->ItemID.ToString(),
		DragOp->DragQuantity,
		DragOp->FromSlotIndex);

	return true;
}


