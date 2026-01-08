#include "CharacterComponent/Inventory_Component.h"
#include "Inventory/DataStruct/DataItem.h"
#include "Inventory/Item/ItemOrigin.h"

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

bool UInventory_Component::IsValidSlot(int32 Index) const
{
    return InventorySlots.IsValidIndex(Index);
}

FInventorySlot* UInventory_Component::GetSlotPtr(int32 Index)
{
    return InventorySlots.IsValidIndex(Index)
        ? &InventorySlots[Index]
        : nullptr;
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

int32 UInventory_Component::FindSlotWithItem(
    const FName& ItemID) const
{
    if (ItemID.IsNone()) return INDEX_NONE;

    for (int32 i = 0; i < InventorySlots.Num(); i++)
    {
        if (InventorySlots[i].ItemID == ItemID)
        {
            return i;
        }
    }

    return INDEX_NONE;
}

bool UInventory_Component::DropItemToWorld(
    int32 SlotIndex, int32 Quantity,
    const FVector& DropLocation,
    const FRotator& DropRotation)
{
    if (!InventorySlots.IsValidIndex(SlotIndex))
        return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    if (Slot.ItemID.IsNone() || Quantity <= 0)
        return false;

    Quantity = FMath::Clamp(Quantity, 1, Slot.Quantity);

    UWorld* World = GetWorld();
    if (!World)
        return false;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AItemOrigin* DroppedItem = World->SpawnActor<AItemOrigin>(
        AItemOrigin::StaticClass(),
        DropLocation,
        DropRotation,
        SpawnParams
    );

    if (!DroppedItem)
        return false;

    // 🔑 FIX UTAMA
    DroppedItem->SetItemDataTable(ItemDataTable);
    DroppedItem->InitializeDrop(Slot.ItemID, Quantity);

    Slot.Quantity -= Quantity;
    if (Slot.Quantity <= 0)
        Slot = FInventorySlot();

    OnInventoryUpdated.Broadcast();
    return true;
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

void UInventory_Component::MoveSlotToSlot(int32 Source, int32 Dest)
{
    UE_LOG(LogTemp, Warning, TEXT("[INVENTORY_COMPONENT] MoveSlotToSlot called: %d -> %d"), Source, Dest);
    
    if (!InventorySlots.IsValidIndex(Source) ||
        !InventorySlots.IsValidIndex(Dest))
        return;

    FInventorySlot& S = InventorySlots[Source];
    FInventorySlot& D = InventorySlots[Dest];

    if (S.IsEmpty()) return;
    if (Source == Dest) return;

    // 1. Slot kosong → pindah
    if (D.IsEmpty())
    {
        D = S;
        S = FInventorySlot();
        OnInventoryUpdated.Broadcast();
        return;
    }

    // 2. Sama jenis → merge
    if (S.ItemID == D.ItemID)
    {
        const FDataItem* Row = GetItemRow(S.ItemID);
        if (Row && Row->ItemNumeric.IsStackable())
        {
            int32 Max = Row->ItemNumeric.MaxStack;
            int32 Total = S.Quantity + D.Quantity;

            D.Quantity = FMath::Min(Max, Total);
            int32 Left = Total - D.Quantity;

            if (Left <= 0)
            {
                S = FInventorySlot(); // kosongkan
            }
            else
            {
                S.Quantity = Left;
            }

            OnInventoryUpdated.Broadcast();
            return;
        }
    }

    // 3. Beda jenis → swap
    InventorySlots.Swap(Source, Dest);
    OnInventoryUpdated.Broadcast();
}

bool UInventory_Component::DropItemBySlotIndex(
    int32 SlotIndex, int32 Quantity)
{
    if (!InventorySlots.IsValidIndex(SlotIndex))
        return false;

    FInventorySlot& Slot = InventorySlots[SlotIndex];
    if (Slot.IsEmpty() || Quantity <= 0)
        return false;

    const FDataItem* Row = GetItemRow(Slot.ItemID);
    if (!Row)
        return false;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AItemOrigin* DroppedItem = GetWorld()->SpawnActor<AItemOrigin>(
        AItemOrigin::StaticClass(),
        GetOwner()->GetActorLocation() +
        GetOwner()->GetActorForwardVector() * 150.f,
        FRotator::ZeroRotator,
        Params
    );

    if (!DroppedItem)
        return false;

    // 🔑 INI YANG SEBELUMNYA HILANG
    DroppedItem->SetItemDataTable(ItemDataTable);
    DroppedItem->InitializeDrop(Slot.ItemID, Quantity);

    Slot.Quantity -= Quantity;
    if (Slot.Quantity <= 0)
        Slot = FInventorySlot();

    OnInventoryUpdated.Broadcast();
    return true;
}

bool UInventory_Component::SplitItem(
    int32 SourceSlot, int32 DestSlot, int32 SplitQuantity)
{
    // ================= VALIDASI DASAR =================
    if (!IsValidSlot(SourceSlot) || !IsValidSlot(DestSlot))
        return false;

    if (SourceSlot == DestSlot)
        return false;

    FInventorySlot& FromSlot = InventorySlots[SourceSlot];
    FInventorySlot& ToSlot   = InventorySlots[DestSlot];

    if (FromSlot.IsEmpty())
        return false;

    // ================= TENTUKAN JUMLAH SPLIT =================
    int32 ActualSplitQty = SplitQuantity;

    // QUICK SPLIT (HALF)
    if (SplitQuantity < 0)
    {
        ActualSplitQty = FromSlot.Quantity / 2;
    }

    // Tidak boleh split 0 / semua
    if (ActualSplitQty <= 0 || ActualSplitQty >= FromSlot.Quantity)
        return false;

    // ================= SLOT TUJUAN =================

    // Case 1: Slot tujuan kosong
    if (ToSlot.IsEmpty())
    {
        ToSlot.ItemID   = FromSlot.ItemID;
        ToSlot.Quantity = ActualSplitQty;

        FromSlot.Quantity -= ActualSplitQty;

        if (FromSlot.Quantity <= 0)
            FromSlot = FInventorySlot();

        OnInventoryUpdated.Broadcast();
        return true;
    }

    // Case 2: Slot tujuan berisi item sama → merge
    if (ToSlot.ItemID == FromSlot.ItemID)
    {
        const FDataItem* Row = GetItemRow(FromSlot.ItemID);
        if (!Row || !Row->ItemNumeric.IsStackable())
            return false;

        int32 MaxStack = Row->ItemNumeric.MaxStack;
        int32 SpaceLeft = MaxStack - ToSlot.Quantity;

        if (SpaceLeft <= 0)
            return false;

        int32 Used = FMath::Min(SpaceLeft, ActualSplitQty);

        ToSlot.Quantity += Used;
        FromSlot.Quantity -= Used;

        if (FromSlot.Quantity <= 0)
            FromSlot = FInventorySlot();

        OnInventoryUpdated.Broadcast();
        return true;
    }

    // Case 3: Item beda → gagal
    return false;
}


