// ItemDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Normal      UMETA(DisplayName = "Normal Item"),
	Food        UMETA(DisplayName = "Food"),
	Crafting    UMETA(DisplayName = "Crafting Material")
};

// Efek makanan (Food)
USTRUCT(BlueprintType)
struct FFoodEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Effect")
	float HPAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Effect")
	float StaminaAmount = 0.0f;
};

UCLASS(BlueprintType)
class THEISLAND_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Constructor
	UItemDataAsset();

	// Getter untuk ID
	FGuid GetID() const { return ID; }

public:
	// Nama item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FName ItemName;

	// Deskripsi item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info", meta=(MultiLine=true))
	FText Description;

	// Tipe item (Normal, Food, Crafting, dll.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Type")
	EItemType ItemType;

	// Apakah item bisa di-stack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking")
	bool bIsStackable = true;

	// Jumlah maksimal stack (jika bIsStackable true)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stacking", meta=(EditCondition="bIsStackable", ClampMin="1"))
	int32 MaxStack = 10;

protected:
	// ID unik yang bisa diedit, tapi juga bisa auto-generate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
	FGuid ID;
};
