#pragma once

#include "CoreMinimal.h"
#include "ItemDataStruct.h"
#include "ConsumableDataStruct.generated.h"

// Child class dari UItemDataStruct khusus item konsumsi
UCLASS(BlueprintType)
class THEISLAND_API UConsumableDataStruct : public UItemDataStruct
{
	GENERATED_BODY()

public:
	// Jumlah HP yang dipulihkan
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable Effect")
	float RestoreHP = 0.f;

	// Jumlah stamina yang dipulihkan
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable Effect")
	float RestoreStamina = 0.f;
};
