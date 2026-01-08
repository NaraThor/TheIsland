#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UDragItemVisual;

UENUM(BlueprintType)
enum class EDragType : uint8
{
	DT_Normal,
	DT_SplitOne,
	DT_HalfSplit
};

UCLASS()
class THEISLAND_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	FName ItemID = NAME_None;
	int32 FromSlotIndex = INDEX_NONE;

	int32 DragQuantity = 1;
	EDragType DragType = EDragType::DT_Normal;

	bool bIsSplitDrag = false;
	bool bDroppedSuccessfully = false;

	// Pointer ke visual widget
	UPROPERTY()
	UDragItemVisual* DragVisual = nullptr;

	bool bDroppedOnSlot = false;

	
};
