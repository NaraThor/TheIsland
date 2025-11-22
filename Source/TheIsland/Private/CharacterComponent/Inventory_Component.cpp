#include "CharacterComponent/Inventory_Component.h"
#include "Inventory/DataStruct/DataItem.h"

// ----------------------------------------------------------
// CONSTRUCTOR
// ----------------------------------------------------------
UInventory_Component::UInventory_Component()
{
    PrimaryComponentTick.bCanEverTick = false;

    
}

// ----------------------------------------------------------
// BEGIN PLAY
// ----------------------------------------------------------
void UInventory_Component::BeginPlay()
{
    Super::BeginPlay();
    InventorySlots.SetNum(InventorySize);

    // Refresh UI langsung di awal
    OnInventoryUpdated.Broadcast();
}

// ----------------------------------------------------------
// GET ITEM ROW
// ----------------------------------------------------------
const FDataItem* UInventory_Component::GetItemRow(const FName& ItemID) const
{
    if (!ItemDataTable || ItemID.IsNone())
        return nullptr;

    return ItemDataTable->FindRow<FDataItem>(ItemID, TEXT("Lookup"));
}

// ----------------------------------------------------------
// FIND STACK
// ----------------------------------------------------------
int32 UInventory_Component::FindExistingStack(const FName& ItemID) const
{
    for (int32 i = 0; i < InventorySlots.Num(); i++)
    {
        const FInventorySlot& Slot = InventorySlots[i];

        if (Slot.ItemID == ItemID)
        {
            const FDataItem* Row = GetItemRow(ItemID);
            if (Row && Slot.Quantity < Row->ItemNumeric.MaxStack)
            {
                UE_LOG(LogTemp, Warning, TEXT("FindExistingStack: checking slot %d | %s x%d (Max: %d)"), i, *Slot.ItemID.ToString(), Slot.Quantity, Row->ItemNumeric.MaxStack);
                return i; // hanya return slot yang belum full
                
            }
        }
    }

    return INDEX_NONE;
}

// ----------------------------------------------------------
// FIND EMPTY SLOT
// ----------------------------------------------------------
int32 UInventory_Component::FindEmptySlot() const
{
    for (int32 i = 0; i < InventorySlots.Num(); i++)
    {
        if (InventorySlots[i].IsEmpty())
            return i;
    }
    return INDEX_NONE;
}

// ----------------------------------------------------------
// ADD ITEM
// ----------------------------------------------------------
FItemAddResult UInventory_Component::HandleAddItem(const FInventorySlot& SlotToAdd)
{
     if (SlotToAdd.ItemID.IsNone() || SlotToAdd.Quantity <= 0)
        return FItemAddResult::AddedNone(FText::FromString("Invalid item"), SlotToAdd.Quantity);

    const FDataItem* Data = GetItemRow(SlotToAdd.ItemID);
    if (!Data)
        return FItemAddResult::AddedNone(FText::FromString("Item not found in DataTable"), SlotToAdd.Quantity);

    int32 Remaining = SlotToAdd.Quantity;

    // ADD TO EXISTING STACK
    if (Data->ItemNumeric.IsStackable())
    {
        int32 StackIndex = FindExistingStack(SlotToAdd.ItemID);
        if (StackIndex != INDEX_NONE)
        {
            FInventorySlot& Slot = InventorySlots[StackIndex];
            int32 SpaceLeft = Data->ItemNumeric.MaxStack - Slot.Quantity;

            int32 ToAdd = FMath::Min(SpaceLeft, Remaining);
            Slot.Quantity += ToAdd;
            Remaining -= ToAdd;

            if (Remaining <= 0)
            {
                OnInventoryUpdated.Broadcast();
                return FItemAddResult::AddedAll(SlotToAdd.Quantity, FText::FromString("All items added to stack"));
            }
        }
    }

    // ADD TO EMPTY SLOT
    while (Remaining > 0)
    {
        int32 EmptyIndex = FindEmptySlot();
        if (EmptyIndex == INDEX_NONE)
        {
            int32 Added = SlotToAdd.Quantity - Remaining;
            OnInventoryUpdated.Broadcast();
            return FItemAddResult::AddedPartial(Added, Remaining, FText::FromString("Inventory Full"));
        }

        int32 ToAdd = Data->ItemNumeric.IsStackable() ? FMath::Min(Data->ItemNumeric.MaxStack, Remaining) : 1;

        InventorySlots[EmptyIndex].ItemID = SlotToAdd.ItemID;
        InventorySlots[EmptyIndex].Quantity = ToAdd;

        Remaining -= ToAdd;

        if (!Data->ItemNumeric.IsStackable())
            break;
    }

    OnInventoryUpdated.Broadcast();
    int32 AddedAmount = SlotToAdd.Quantity - Remaining;
    if (Remaining > 0)

    return FItemAddResult::AddedPartial(AddedAmount, Remaining, FText::FromString("Some items added, inventory full"));
    return FItemAddResult::AddedAll(AddedAmount, FText::FromString("Item added successfully"));
}

// ----------------------------------------------------------
// DROP ITEM
// ----------------------------------------------------------
bool UInventory_Component::DropItem(const FName& ItemID, int32 Quantity)
{
    if (Quantity <= 0) return false;

    int32 SlotIndex = FindExistingStack(ItemID);
    if (SlotIndex == INDEX_NONE) return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];

    if (Slot.Quantity < Quantity) return false;

    Slot.Quantity -= Quantity;

    if (Slot.Quantity <= 0)
    {
        Slot.ItemID = NAME_None;
        Slot.Quantity = 0;
    }

    OnInventoryUpdated.Broadcast();
    return true;
}
