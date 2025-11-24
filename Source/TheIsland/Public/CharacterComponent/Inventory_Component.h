#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Inventory/DataStruct/DataItem.h"
#include "Inventory_Component.generated.h"

// ----------------------------------------------------------
// DELEGATE
// ----------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

// ----------------------------------------------------------
// ENUM HASIL ADD ITEM
// ----------------------------------------------------------
UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
    IAR_NoItemAdded,
    IAR_PartialAmountItemAdded,
    IAR_AllItemAdded
};

// ----------------------------------------------------------
// DATA RESULT ADD ITEM
// ----------------------------------------------------------
USTRUCT(BlueprintType)
struct FItemAddResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 ActualAmountAdded = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 RemainingAmount = 0;         // jumlah yang tidak masuk inventory

    UPROPERTY(BlueprintReadOnly)
    EItemAddResult OperationResult = EItemAddResult::IAR_NoItemAdded;

    UPROPERTY(BlueprintReadOnly)
    FText ResultMessage;

    static FItemAddResult AddedNone(const FText& Msg, int32 Quantity)
    {
        return {0, Quantity, EItemAddResult::IAR_NoItemAdded, Msg};
    }

    static FItemAddResult AddedPartial(int32 Added, int32 Remaining, const FText& Msg)
    {
        return {Added, Remaining, EItemAddResult::IAR_PartialAmountItemAdded, Msg};
    }

    static FItemAddResult AddedAll(int32 Amount, const FText& Msg)
    {
        return {Amount, 0, EItemAddResult::IAR_AllItemAdded, Msg};
    }
};

// ----------------------------------------------------------
// DATA ITEM (ROW TABLE)
// ----------------------------------------------------------
USTRUCT(BlueprintType)
struct FInventoryItemRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxStack = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsStackable = false;
};

// ----------------------------------------------------------
// SLOT INVENTORY
// ----------------------------------------------------------
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemID = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Quantity = 0;

    bool IsEmpty() const { return ItemID.IsNone() || Quantity <= 0; }
};

// ----------------------------------------------------------
// COMPONENT INVENTORY
// ----------------------------------------------------------
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UInventory_Component : public UActorComponent
{
    GENERATED_BODY()

public:

    UInventory_Component();

protected:
    virtual void BeginPlay() override;

public:

    // -----------------------------
    // SETUP
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
    int32 InventorySize = 16;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
    TArray<FInventorySlot> InventorySlots;

    // -----------------------------
    // API
    // -----------------------------
    UFUNCTION(BlueprintCallable, Category="Inventory")
    const TArray<FInventorySlot>& GetInventoryContents() const { return InventorySlots; }

    UFUNCTION(BlueprintCallable, Category="Inventory")
    FItemAddResult HandleAddItem(const FInventorySlot& SlotToAdd);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool DropItem(const FName& ItemID, int32 Quantity);
    void MoveSlotToSlot(int32 Source, int32 Dest);

    // -----------------------------
    // Helpers
    // -----------------------------
    const FDataItem* GetItemRow(const FName& ItemID) const;
    int32 FindExistingStack(const FName& ItemID) const;
    int32 FindEmptySlot() const;

    // -----------------------------
    // EVENT UI
    // -----------------------------
    UPROPERTY(BlueprintAssignable)
    FOnInventoryUpdated OnInventoryUpdated;
};
