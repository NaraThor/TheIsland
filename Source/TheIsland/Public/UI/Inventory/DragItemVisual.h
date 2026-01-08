#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

struct FInventorySlot;
struct FDataItem;
class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class THEISLAND_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitVisual(
			const FInventorySlot& InSlot,
			const FDataItem* InItemRow
		);
	
protected:
	
	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UTextBlock* ItemQuantity;
};