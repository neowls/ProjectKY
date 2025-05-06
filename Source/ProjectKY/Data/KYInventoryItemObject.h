#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Struct/KYStruct.h"
#include "ProjectKY.h"
#include "KYInventoryItemObject.generated.h"


UCLASS()
class PROJECTKY_API UKYInventoryItemObject : public UObject, public IUserObjectListEntry
{
	GENERATED_BODY()

	
public:
	TWeakPtr<FKYItemData> GetItemRef() const { return WeakRef; }
	TSharedPtr<FKYItemData> GetItemPinned() const { return WeakRef.Pin(); }

	
	
	void Initialize(const TWeakPtr<FKYItemData>& InRef)
	{
		WeakRef = InRef;
		KY_LOG(LogKY, Warning, TEXT("InRef : %s"), *InRef.Pin()->Name.ToString());
	}

	void SetData(const TSharedPtr<FKYItemData>& InData)
	{
		WeakRef = InData;
	}
	
	void ClearData()
	{
		WeakRef.Reset();
	}

	void SetInHand(bool bInHand) { bIsInHand = bInHand; }
	
	bool IsInHand() const { return bIsInHand; }
	
	bool IsEmpty() const { return WeakRef.Pin()->IsEmpty(); }


private:
	TWeakPtr<FKYItemData> WeakRef;
	
	UPROPERTY()
	bool bIsInHand = false;
};
