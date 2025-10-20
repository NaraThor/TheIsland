#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseItem.generated.h"

// ---------------------- ENUM ----------------------

UENUM(BlueprintType)
enum class EItemTypeData : uint8
{
	Material UMETA(DisplayName = "Material"),
	Consumable UMETA(DisplayName = "Consumable"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Quest UMETA(DisplayName = "Quest")
};

UENUM(BlueprintType)
enum class EItemRarityData : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Mythic UMETA(DisplayName = "Mythic"),
	Legendary UMETA(DisplayName = "Legendary")
};

// ---------------------- SUB STRUCT ----------------------

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Text")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Text")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Text")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Text")
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FItemStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Stats")
	float Defend = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Stats")
	float Attack = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Stats")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Stats")
	float HealAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Stats")
	float SellValue = 0.f;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Numeric")
	float Weight = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Numeric")
	int32 MaxStack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Numeric")
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Asset")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Asset")
	UStaticMesh* Mesh = nullptr;
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|Asset")
	//FVector MeshScale = FVector(1.f, 1.f, 1.f);
};

// ---------------------- FINAL ITEM STRUCT ----------------------

USTRUCT(BlueprintType)
struct FBaseItem : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|BaseData")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|BaseData")
	EItemTypeData ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|BaseData")
	EItemRarityData ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|TextData")
	FItemTextData ItemText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|StatsData")
	FItemStatData ItemStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|NumericData")
	FItemNumericData ItemNumeric;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item|AssetData")
	FItemAssetData ItemAsset;
};