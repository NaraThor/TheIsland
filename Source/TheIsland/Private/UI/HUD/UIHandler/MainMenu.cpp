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
	UItemDragDropOperation* DragOp =
		Cast<UItemDragDropOperation>(InOperation);

	// invalid / sudah dihandle slot
	if (!DragOp || DragOp->bDroppedSuccessfully)
	{
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

	// 🔥 DROP KE WORLD
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


