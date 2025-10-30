#pragma once

#include "CoreMinimal.h"
#include "DataItem.h"
#include "Engine/DataTable.h"
#include "FoodItem.generated.h"

USTRUCT(BlueprintType)
struct FFoodItem : public FDataItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount;
};