#pragma once
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Struct/KYStruct.h"
#include "KYInventoryItemObject.generated.h"

UCLASS()
class UKYInventoryItemObject : public UObject, public IUserObjectListEntry
{
	GENERATED_BODY()

	
public:
	TWeakPtr<FKYItemData> GetItemRef() const { return WeakRef; }
	TSharedPtr<FKYItemData> GetItemPinned() const { return WeakRef.Pin(); }
	
	void Initialize(const TWeakPtr<FKYItemData>& InRef)
	{
		WeakRef = InRef;
		bIsEmpty = false;
	}

	void InitializeEmptyArmor(EKYArmorSubType NewArmorSlot)
	{
		ArmorSlot = NewArmorSlot;
		bIsEmpty = true;
	}

	void InitializeEmptyWeapon(uint8 NewSlotIndex)
	{
		WeaponSlotIndex = NewSlotIndex;
		bIsEmpty = true;
	}

	void SetData(const TSharedPtr<FKYItemData>& InData)
	{
		WeakRef = InData;
		bIsEmpty = InData == nullptr;
	}
	
	void ClearData()
	{
		WeakRef.Reset();
		bIsEmpty = true;
	}
	
	EKYArmorSubType ArmorSlot = EKYArmorSubType::None;
	uint8 WeaponSlotIndex = 255;
	
	bool bIsInHand = false;

	bool IsEmpty() const { return bIsEmpty; }

	void SetArmorSlotType(EKYArmorSubType InSlot) { ArmorSlot = InSlot; }
	void SetWeaponSlotIndex(uint8 Index) { WeaponSlotIndex = Index; }

private:
	TWeakPtr<FKYItemData> WeakRef;

	UPROPERTY()
	bool bIsEmpty = true;
};
