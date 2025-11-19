#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CharacterComponent/Inventory_Component.h"
#include "ItemDragDropOperation.generated.h"


class UBaseItem;
class UInventory_Component;

UCLASS()
class THEISLAND_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public :
	// ID item yang sedang di-drag
		UPROPERTY(BlueprintReadWrite, Category="Inventory")
	FName ItemID = NAME_None;

	// Jumlah item yang sedang di-drag
	UPROPERTY(BlueprintReadWrite, Category="Inventory")
	int32 DragQuantity = 1;

	// Slot yang sedang di-drag
	UPROPERTY()
	FInventorySlot SlotData;

	// Referensi ke inventory source (opsional, untuk drop kembali ke inventory asal)
	UPROPERTY(BlueprintReadWrite, Category="Inventory")
	class UInventory_Component* SourceInventory = nullptr;
	
};