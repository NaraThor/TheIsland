#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataCharacter.generated.h"

// ---------------------- ENUM ----------------------

UENUM(BlueprintType)
enum class ECharacterTypeData : uint8
{
	Player UMETA(DisplayName = "Player"), 
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"), //pet / companion?
	Enemy UMETA(DisplayName = "Enemy"),
	Animal UMETA(DisplayName = "Animal"),
	Unique UMETA(DisplayName = "Unique")
};

// ---------------------- SUB STRUCT ----------------------

USTRUCT(BlueprintType)
struct FCharacterTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Text")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Text")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Text")
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Text")
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FCharacterStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Stats")
	float Defend = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Stats")
	float Attack = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Stats")
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Stats")
	float HealAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Stats")
	float SellValue = 0.f;
};

USTRUCT(BlueprintType)
struct FCharacterNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Numeric")
	float MaxCapapsityWeight = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Numeric")
	int32 MaxCapapsityStack = 8;
};

USTRUCT(BlueprintType)
struct FCharacterAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Asset")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Asset")
	UStaticMesh* Mesh = nullptr;
};

// ---------------------- FINAL ITEM STRUCT ----------------------

USTRUCT(BlueprintType)
struct FDataCharacter : public FTableRowBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	ECharacterTypeData CharacterType = ECharacterTypeData::NonPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	FCharacterTextData CharacterText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	FCharacterStatData CharacterStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	FCharacterNumericData CharacterNumeric;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|BaseData")
	FCharacterAssetData CharacterAsset;
	
};

