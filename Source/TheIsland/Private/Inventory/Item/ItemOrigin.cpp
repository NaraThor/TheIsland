#include "Inventory/Item/ItemOrigin.h"
#include "Components/StaticMeshComponent.h"
#include "Character/CharacterOrigin.h"
#include "Engine/DataTable.h"
#include "Inventory/DataStruct/DataItem.h"
#include "CharacterComponent/Inventory_Component.h"

AItemOrigin::AItemOrigin()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetSimulatePhysics(true);
}

/* ============================================================
   INITIALIZATION
   ============================================================ */

void AItemOrigin::InitializePickup()
{
	if (!ItemRowHandle.DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemOrigin: Missing DataTable!"));
		return;
	}

	// Row valid?
	const FDataItem* Row = nullptr;

	if (!ItemRowHandle.RowName.IsNone())
	{
		Row = ItemRowHandle.GetRow<FDataItem>("InitializePickup");
	}

	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemOrigin: Invalid RowHandle for pickup actor"));
		return;
	}

	// Set ItemID based on data
	ItemID = Row->ID;

	// ---- QUANTITY LOGIC ----
	// Jika Quantity belum diset (0), artinya menggunakan default row
	if (Quantity <= 0)
	{
		// Bisa pilih salah satu:

		// 1) Spawn default = 1 (ala game survival)
		Quantity = 1;

		// 2) ATAU spawn default full stack (aktifkan jika mau)
		// Quantity = Row->ItemNumeric.MaxStack;
	}
	else
	{
		// Jika designer isi manual (1–MaxStack), kita clamp supaya aman
		Quantity = FMath::Clamp(Quantity, 1, Row->ItemNumeric.MaxStack);
	}

	// Update mesh dan info visual
	UpdateVisualFromData(Row);

	UE_LOG(LogTemp, Log,
		TEXT("Initialized Pickup: %s | Quantity = %d | MaxStack = %d"),
		*ItemID.ToString(), Quantity, Row->ItemNumeric.MaxStack);
}

void AItemOrigin::InitializeDrop(FName InItemID, int32 InQuantity)
{
	bIsDroppedItem = true;

	ItemID   = InItemID;
	Quantity = InQuantity;

	if (!ItemRowHandle.DataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Drop failed: Missing DataTable"));
		return;
	}

	ItemRowHandle.RowName = ItemID;

	const FDataItem* Row =
		ItemRowHandle.GetRow<FDataItem>("InitializeDrop");

	if (!Row)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Drop: Item ID '%s' not found"), *ItemID.ToString());
		return;
	}

	Quantity = FMath::Clamp(Quantity, 1, Row->ItemNumeric.MaxStack);

	UpdateVisualFromData(Row);
}

/* ============================================================
   VISUAL UPDATE
   ============================================================ */

void AItemOrigin::UpdateVisualFromData(const FDataItem* ItemData)
{
	if (!ItemData)
		return;

	if (ItemData->ItemAsset.Mesh)
		MeshComponent->SetStaticMesh(ItemData->ItemAsset.Mesh);
}

/* ============================================================
   INTERACTION
   ============================================================ */

void AItemOrigin::BeginFocus()
{
	
}

void AItemOrigin::EndFocus()
{
	
}

void AItemOrigin::Interact(ACharacterOrigin* PlayerCharacter)
{
	if (!PlayerCharacter)
		return;

	TakePickup(PlayerCharacter);
}

void AItemOrigin::TakePickup(ACharacterOrigin* Taker)
{
	if (IsPendingKillPending())
		return;

	if (ItemID.IsNone() || Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pickup failed: invalid ItemID/Quantity"));
		return;
	}

	UInventory_Component* PlayerInventory = Taker->GetInventory();
	if (!PlayerInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player has no Inventory_Component"));
		return;
	}

	FInventorySlot SlotToAdd;
	SlotToAdd.ItemID = ItemID;
	SlotToAdd.Quantity = Quantity;

	const FItemAddResult AddResult = PlayerInventory->HandleAddItem(SlotToAdd);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());

	switch (AddResult.OperationResult)
	{
	case EItemAddResult::IAR_NoItemAdded:
		UE_LOG(LogTemp, Warning, TEXT("No items added"));
		break;

	case EItemAddResult::IAR_PartialAmountItemAdded:
		Quantity = AddResult.RemainingAmount; // sisanya tetap di pickup
		UE_LOG(LogTemp, Warning, TEXT("Partial add, remaining: %d"), Quantity);
		break;

	case EItemAddResult::IAR_AllItemAdded:
		Destroy(); // semua berhasil masuk inventory
		break;
	}
}

/* ============================================================
   ENGINE
   ============================================================ */

void AItemOrigin::SetItemDataTable(
	UDataTable* InDataTable)
{
	if (!InDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemOrigin: SetItemDataTable nullptr"));
		return;
	}

	ItemRowHandle.DataTable = InDataTable;
}

void AItemOrigin::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsDroppedItem)
	{
		// Initial untuk spawn natural
		InitializePickup();
	}
}

/* ============================================================
   EDITOR
   ============================================================ */

#if WITH_EDITOR
void AItemOrigin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!ItemRowHandle.DataTable)
		return;

	const FName ChangedProp = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	// Ambil row sekali saja
	const FDataItem* Row = nullptr;
	if (!ItemRowHandle.RowName.IsNone())
	{
		Row = ItemRowHandle.GetRow<FDataItem>("EditorUpdate");
	}

	// === RowHandle berubah ===
	if (ChangedProp == GET_MEMBER_NAME_CHECKED(AItemOrigin, ItemRowHandle))
	{
		if (Row)
		{
			ItemID = Row->ID;
			Quantity = FMath::Clamp(Quantity, 1, Row->ItemNumeric.MaxStack);

			UpdateVisualFromData(Row);
		}
		else
		{
			// Jika row hilang → reset item
			ItemID = NAME_None;
			Quantity = 1;
		}
	}

	// === Quantity berubah ===
	if (ChangedProp == GET_MEMBER_NAME_CHECKED(AItemOrigin, Quantity))
	{
		if (Row)
		{
			// Jika kamu ingin bisa 0, ganti 1 → 0 di sini
			Quantity = FMath::Clamp(Quantity, 1, Row->ItemNumeric.MaxStack);
		}
	}
}
#endif