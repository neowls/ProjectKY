#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Struct/KYStruct.h"
#include "KYInventoryItemObject.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnChanged, const FKYItemData&, ItemData);

UCLASS()
class PROJECTKY_API UKYInventoryItemObject : public UObject, public IUserObjectListEntry
{
	GENERATED_BODY()


	
public:
	FOnChanged OnChanged;
	const TWeakPtr<FKYItemData>& GetItemRef()
	{
		return ItemData;
	}

	TSharedPtr<FKYItemData> GetItemPinned() const
	{
		return ItemData.Pin();
	}

	FKYItemData GetItemData() const
	{
		if (GetItemPinned()) return *GetItemPinned();
		return FKYItemData();
	}

	bool IsEmpty() const
	{
		return GetItemData().IsEmpty();
	}
	
	void SetData(const TSharedPtr<FKYItemData>& InData)
	{
		if (!InData.IsValid()) return;
		ItemData = InData;
		HandleDataChanged();
	}

	void HandleDataChanged() const
	{
		OnChanged.ExecuteIfBound(GetItemData());
	}
	
	void ClearData()
	{
		ItemData.Reset();
	}



private:
	TWeakPtr<FKYItemData> ItemData;
	
};
