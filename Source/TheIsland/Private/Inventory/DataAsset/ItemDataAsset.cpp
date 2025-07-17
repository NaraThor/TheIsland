


#include "Inventory/DataStruct/DataAsset/ItemDataAsset.h"

UItemDataAsset::UItemDataAsset()
{
	// Set ID jika belum pernah diisi
	if (!ID.IsValid())
	{
		ID = FGuid::NewGuid();
	}

	// Set default type untuk menghindari warning dan bug
	ItemType = EItemType::Normal; // Jika enum kamu punya opsi "None"
}