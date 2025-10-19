// InventorySlot.h
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	FInventorySlot()
		: ItemID(NAME_None), Quantity(0) {}
};
