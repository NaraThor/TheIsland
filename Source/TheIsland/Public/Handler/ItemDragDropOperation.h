#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CharacterComponent/Inventory_Component.h"
#include "ItemDragDropOperation.generated.h"

class UBaseItem;
class UInventory_Component;

UENUM(BlueprintType)
enum class EDragType : uint8
{
	DT_Normal, 
	DT_Adjust, //Scroll
	DT_Split //Ctrl Split
};

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

	// SLOT INDEX (baru) — index slot sumber di inventory (0..N)
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 SlotIndex = -1;

	// Referensi ke inventory source (opsional, untuk drop kembali ke inventory asal)
	UPROPERTY(BlueprintReadWrite, Category="Inventory")
	class UInventory_Component* SourceInventory = nullptr;

	UPROPERTY(BlueprintReadWrite, Category="Inventory")
	EDragType DragType = EDragType::DT_Normal;

	// TRUE jika ini adalah drag hasil split (bukan drag normal)
	UPROPERTY(BlueprintReadWrite)
	bool bIsSplitDrag = false;

	// Jumlah item hasil split
	UPROPERTY(BlueprintReadWrite)
	int32 SplitAmount = 0;
	
};