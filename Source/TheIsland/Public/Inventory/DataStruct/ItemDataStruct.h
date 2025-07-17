#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemTypeStruct.h"
#include "ItemDataStruct.generated.h"

// Base data asset untuk semua item
UCLASS(BlueprintType)
class THEISLAND_API UItemDataStruct : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ID unik untuk setiap item (boleh manual atau auto assign di BP)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemID;

	// Nama item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	UTexture2D* ItemIcon;

	// Deskripsi item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FText Description;

	// Tipe item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemTypeUniversal Category;

	// Apakah item bisa distack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stacking")
	bool bIsStackable = true;

	// Max stack (jika bisa distack)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stacking", meta = (EditCondition = "bIsStackable", ClampMin = 1))
	int32 MaxStack = 1;
};
