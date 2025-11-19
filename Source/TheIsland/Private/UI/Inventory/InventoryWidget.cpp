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
	if (!PlayerCharacter) return;

	InventoryReference = PlayerCharacter->GetInventory();
	if (!InventoryReference) return;

	InventoryReference->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::RefreshInventory);

	// Debug log
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget Constructed: %s | Parent: %s | InViewport: %d | Outer: %s | OwningPawn: %s"),
		*GetNameSafe(this),
		*GetNameSafe(GetParent()),
		IsInViewport() ? 1 : 0,
		*GetNameSafe(GetOuter()),
		*GetNameSafe(GetOwningPlayerPawn()));

	UE_LOG(LogTemp, Warning, TEXT("UI Refresh From Inventory Work!!! %s"), *GetName());
}


void UInventoryWidget::RefreshInventory()
{

	if (!InventoryReference || !InventorySlotClass) return;

	InventoryPanel->ClearChildren();

	for (const FInventorySlot& InventorySlot : InventoryReference->GetInventoryContents())
	{
		if (InventorySlot.IsEmpty()) continue;

		const FDataItem* ItemRow = InventoryReference->GetItemRow(InventorySlot.ItemID);
		if (!ItemRow) continue;

		UInventorySlotWidget* ItemSlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);
		ItemSlotWidget->SetItemData(InventorySlot, ItemRow);

		InventoryPanel->AddChild(ItemSlotWidget);
	}
	
}


bool UInventoryWidget::NativeOnDrop(
	const FGeometry& InGeometry,const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if (!ItemDragDrop || InventoryReference == nullptr)
		return false;

	FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FGeometry PanelGeometry = InventoryPanel->GetCachedGeometry();

	bool bIsInsideInventoryPanel = PanelGeometry.IsUnderLocation(InDragDropEvent.GetScreenSpacePosition());

	if (bIsInsideInventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop dilakukan di dalam inventory panel"));
		// TODO: swap logic jika di dalam panel
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop dilakukan di luar inventory -> jatuhkan ke dunia"));
		InventoryReference->DropItem(ItemDragDrop->ItemID, ItemDragDrop->DragQuantity);
	}

	return true;
}