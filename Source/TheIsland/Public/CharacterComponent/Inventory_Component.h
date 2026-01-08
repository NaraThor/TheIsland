#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Inventory/Data/InventoryTypes.h"
#include "Inventory/DataStruct/DataItem.h"
#include "Inventory_Component.generated.h"

// ----------------------------------------------------------
// DELEGATE
// ----------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);


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

    //HELPER..
    // Validasi index
    bool IsValidSlot(int32 Index) const;
    FInventorySlot* GetSlotPtr(int32 Index);
    
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

    void MoveSlotToSlot(int32 Source, int32 Dest);

    bool DropItemBySlotIndex(int32 SlotIndex, int32 Quantity);
   
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool SplitItem(int32 SourceSlot, int32 DestSlot, int32 SplitQuantity);
    
    // -----------------------------
    // Helpers
    // -----------------------------
    const FDataItem* GetItemRow(const FName& ItemID) const;
    int32 FindExistingStack(const FName& ItemID) const;
    int32 FindEmptySlot() const;
    int32 FindSlotWithItem(const FName& ItemID) const;
    
    // -----------------------------
    // EVENT UI
    // -----------------------------
    UPROPERTY(BlueprintAssignable)
    FOnInventoryUpdated OnInventoryUpdated;

    UFUNCTION()
    bool DropItemToWorld(int32 SlotIndex,int32 Quantity,const FVector& DropLocation,
    const FRotator& DropRotation);

};
