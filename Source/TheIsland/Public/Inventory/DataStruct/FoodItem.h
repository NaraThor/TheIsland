#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Engine/DataTable.h"
#include "FoodItem.generated.h"

USTRUCT(BlueprintType)
struct FFoodItem : public FBaseItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount;
};