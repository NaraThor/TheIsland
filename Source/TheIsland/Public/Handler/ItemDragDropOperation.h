#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

UENUM(BlueprintType)
enum class EDragType : uint8
{
	DT_Normal,
	DT_Split
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
};
