#include "Inventory/Item/BaseItem.h"

UBaseItem::UBaseItem() : bIsCopy(false),  bIsPickup(false)
{
	
}

void UBaseItem::ResetItemFlag()
{
	bIsCopy = false;
	bIsPickup = false;
}

UBaseItem* UBaseItem::CreateItemCopy() const
{

	UBaseItem* ItemCopy = NewObject<UBaseItem>(StaticClass());

	ItemCopy -> ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemRarity = this->ItemRarity;
	ItemCopy->ItemText= this->ItemText;
	ItemCopy->ItemStat = this->ItemStat;
	ItemCopy->ItemNumeric = this->ItemNumeric;
	ItemCopy->ItemAsset= this->ItemAsset;

	ItemCopy -> bIsCopy = true;

	return ItemCopy;
}


void UBaseItem::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != this -> Quantity)
	{
		//FMath::Clamp(A[Nilai rill], MIN, MAX);
		Quantity = FMath::Clamp(NewQuantity, 0,
		this -> ItemNumeric.IsStackable()? this->ItemNumeric.MaxStack : 1);

		if (this -> OwningInventory)
		{
			if (this -> Quantity <= 0)
			{
				//RemoveSingleInstanceOfItem Inventory Function
				//this->OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("ItemBase Owning Inventory Was Null (item may be a pickup)."));
		}
	}
}

void UBaseItem::Use(ACharacterOrigin* Character)
{
}
