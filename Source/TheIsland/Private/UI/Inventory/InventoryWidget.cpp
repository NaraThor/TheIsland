#include "UI/Inventory/InventoryWidget.h"

#include "Character/CharacterOrigin.h"
#include "CharacterComponent/Inventory_Component.h"
#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "Handler/ItemDragDropOperation.h"
#include "Inventory/Item/BaseItem.h"
#include "UI/Inventory/InventorySlotWidget.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACharacterOrigin>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			
			InventoryReference->OnInventoryUpdated.AddUObject(this,&UInventoryWidget::RefreshInventory);

			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget Constructed: %s | Parent: %s | InViewport: %d | Outer: %s | OwningPawn: %s"),
		*GetNameSafe(this),
		*GetNameSafe(GetParent()),
		IsInViewport() ? 1 : 0,
		*GetNameSafe(GetOuter()),
		*GetNameSafe(GetOwningPlayerPawn()));
			UE_LOG(LogTemp,Warning,TEXT("UI Refresh From Inventory Work!!! %s"), *GetName());
		}
	}
}


void UInventoryWidget::RefreshInventory()
{

	if (InventoryReference&& InventorySlotClass)
	{
		InventoryPanel -> ClearChildren();

		for (UBaseItem* const& InventoryItem:InventoryReference->GetInventoryContents())
		{
			UInventorySlotWidget* ItemSlot = CreateWidget<UInventorySlotWidget>(this,InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChild(ItemSlot);
		}
		//Nambah command
	}
	
}


bool UInventoryWidget::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem&& InventoryReference)
	{
		UE_LOG(LogTemp,Warning,TEXT("Detect on Item Drop on Inventory Panel"));
		return true;
	}
	return false;
}