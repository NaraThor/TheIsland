#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"


class UBaseItem;
class UInventory_Component;

UCLASS()
class THEISLAND_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public :
	UPROPERTY()
	UBaseItem* SourceItem;

	UPROPERTY()
	UInventory_Component* SourceInventory;
	
};