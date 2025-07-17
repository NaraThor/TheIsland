#pragma once

#include "CoreMinimal.h"
#include "ItemTypeStruct.generated.h"

// ENUM untuk jenis item
UENUM(BlueprintType)
enum class EItemTypeUniversal : uint8
{
	Normal		UMETA(DisplayName = "Normal"),
	Food		UMETA(DisplayName = "Food"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Attachment	UMETA(DisplayName = "Attachment"),
	Crafting	UMETA(DisplayName = "Crafting")
};
