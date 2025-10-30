// Inventory_Component.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory_Component.generated.h"

// Delegate untuk notifikasi update
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UBaseItem;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No Item Added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial Amount Item Added"),
	IAR_AllItemAdded UMETA(DisplayName = "All Item Added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult():
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone (const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
		
	};
	static FItemAddResult AddedAll(const int32 AmountToAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountToAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
		
	};
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEISLAND_API UInventory_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	///////////// VARIABLE ///////////
	FOnInventoryUpdated OnInventoryUpdated;


	///////////// FUNCTION ///////////
	UInventory_Component();

	UFUNCTION(Category="Inventory")
	FItemAddResult HandleAddItem(UBaseItem* InputItem);

	UFUNCTION(Category="Inventory")
	UBaseItem* FindMatchingItem(UBaseItem* ItemIn) const;
	UFUNCTION(Category="Inventory")
	UBaseItem* FindNextItemByID(UBaseItem* ItemIn) const;
	UFUNCTION(Category="Inventory")
	UBaseItem* FindNextPartialStack(UBaseItem* ItemIn) const;

	UFUNCTION(Category="Inventory")
	void RemoveSingleInstanceOfItem(UBaseItem* ItemToRemove);
	UFUNCTION(Category="Inventory")
	int32 RemoveAmountOfItem(UBaseItem* ItemIn,int32 DesiredAmountToRemove);
	UFUNCTION(Category="Inventory")
	void SplitExistingStack(UBaseItem* ItemIn, const int32 AmountToSplit);
	
	/////////////////Getteers////////////////////////////
	UFUNCTION(Category="Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const{return InventoryTotalWeight; };
	UFUNCTION(Category="Inventory")
	FORCEINLINE float GetWeightCapacity() const{return InventoryWeightCapacity; };
	UFUNCTION(Category="Inventory") // nanti dibuat modular per char  
	FORCEINLINE int32 GetSlotCapacity() const{return InventorySlotCapacity; };
	UFUNCTION(Category="Inventory") // nanti dibuat modular per char
	
	FORCEINLINE TArray<UBaseItem*> GetInventoryContents() const {return InventoryContents; };

	/////////////////Setteers////////////////////////////
	UFUNCTION(Category="Inventory")
	FORCEINLINE void SetSlotCapacity(const int32 NewSlotsCapacity)
	{
		InventorySlotCapacity = FMath::Max(1, NewSlotsCapacity);
	}

	UFUNCTION(Category="Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity)
	{
		InventoryWeightCapacity = FMath::Max(0.0f, NewWeightCapacity);
	}
	
	int32 GetEmptySlotCount() const;
	bool HasEmptySlot() const;

protected:
	///////////// VARIABLE ///////////
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	int32 InventorySlotCapacity;
	UPROPERTY(EditInstanceOnly, Category = "Inventory")
	float InventoryWeightCapacity;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UBaseItem>> InventoryContents;
	

	
	///////////// FUNCTION ///////////
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UBaseItem* InputItem);
	int32 HandleStackableItems(UBaseItem* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UBaseItem* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UBaseItem* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem (UBaseItem* Item, const int32 AmountToAdd);
};
