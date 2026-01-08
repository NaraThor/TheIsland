#include "UI/Inventory/InventoryWidget.h"

#include "Character/CharacterOrigin.h"
#include "CharacterComponent/Inventory_Component.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/InventorySlotWidget.h"
#include "Handler/ItemDragDropOperation.h"

void UInventoryWidget::DropItemToWorld(
	int32 FromSlotIndex, int32 Quantity)
{
	if (!InventoryRef || !PlayerCharacter)
		return;

	InventoryRef->DropItemToWorld(
		FromSlotIndex,
		Quantity,
		PlayerCharacter->GetActorLocation() +
		PlayerCharacter->GetActorForwardVector() * 150.f,
		PlayerCharacter->GetActorRotation()
	);
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACharacterOrigin>(GetOwningPlayerPawn());
	if (!PlayerCharacter) return;

	InventoryRef = PlayerCharacter->GetInventory();
	if (!InventoryRef) return;

	InventoryRef->OnInventoryUpdated.AddDynamic(
		this, &UInventoryWidget::RefreshInventory);

	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	if (!InventoryRef || !InventoryPanel || !InventorySlotClass)
		return;

	InventoryPanel->ClearChildren();

	const TArray<FInventorySlot>& Slots =
		InventoryRef->GetInventoryContents();

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		const FInventorySlot& SlotData = Slots[i];

		const FDataItem* Row =
			SlotData.IsEmpty() ? nullptr :
			InventoryRef->GetItemRow(SlotData.ItemID);

		UInventorySlotWidget* SlotWidget =
			CreateWidget<UInventorySlotWidget>(this, InventorySlotClass);

		if (!SlotWidget) continue;

		SlotWidget->InitSlot(i, SlotData, Row, this);
		InventoryPanel->AddChild(SlotWidget);
	}
}

void UInventoryWidget::HandleSlotDrop(
	UInventorySlotWidget* TargetSlot,UItemDragDropOperation* DragOp)
{
	if (!InventoryRef || !TargetSlot || !DragOp)
		return;

	const int32 From = DragOp->FromSlotIndex;
	const int32 To   = TargetSlot->GetSlotIndex();

	if (From == To)
	{
		DragOp->bDroppedSuccessfully = false;
		RefreshInventory(); 
		return;
	}

	switch (DragOp->DragType)
	{
	case EDragType::DT_Normal:
		InventoryRef->MoveSlotToSlot(From, To);
		break;

	case EDragType::DT_HalfSplit:
		InventoryRef->SplitItem(
			From, To, DragOp->DragQuantity);
		break;

	case EDragType::DT_SplitOne:
		InventoryRef->SplitItem(
			From, To, DragOp->DragQuantity);
		break;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("DragType: %d"), (int32)DragOp->DragType);
	DragOp->bDroppedSuccessfully = true;
	RefreshInventory();
}
