#pragma once

#include "CoreMinimal.h"
#include "Inventory/DataStruct/DataItem.h"
#include "BaseItem.generated.h"

class ACharacterOrigin;
class UInventory_Component;

/**
 * UItem_Origin adalah representasi data item di runtime.
 * - Tidak muncul di dunia (bukan Actor)
 * - Disimpan di Inventory
 * - Mengambil data statis dari DataTable (FItemDataRow)
 */

UCLASS(Blueprintable, BlueprintType)
class THEISLAND_API UBaseItem : public UObject
{
	GENERATED_BODY()

public:

	////////////properties

	UPROPERTY()
	UInventory_Component* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category = "Item") //"meta=(UIMin=1, UIMax=99)" set min max Value
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemTypeData ItemType;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemRarityData ItemRarity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData ItemText;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatData ItemStat;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData ItemNumeric;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData ItemAsset;

	bool bIsCopy;
	bool bIsPickup;

	//////////////Function

	UBaseItem();

	void ResetItemFlag();

	UFUNCTION(Category="Item")
	UBaseItem* CreateItemCopy() const;

	UFUNCTION(Category="Item")
	FORCEINLINE bool IsItemFullStack() const {return Quantity == ItemNumeric.MaxStack;}

	UFUNCTION(Category="Item")
	FORCEINLINE float GetItemSingleWeight() const {return ItemNumeric.Weight;}

	//UFUNCTION(Category="Item")
	//FORCEINLINE float GetItemStackWeight() const {return Quantity * ItemNumeric.Weight;}

	UFUNCTION(Category="Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category="Item")
	virtual void Use(ACharacterOrigin* Character);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this -> ID == OtherID;	
	}
	
};