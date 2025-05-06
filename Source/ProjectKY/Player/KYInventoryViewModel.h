#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Struct/KYStruct.h"
#include "KYInventoryViewModel.generated.h"

class AKYPlayerState;

UCLASS()
class PROJECTKY_API UKYInventoryViewModel : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(AKYPlayerState* InPlayerState);

	TArray<TSharedPtr<FKYItemData>> GetFilteredInventory(EKYItemType Category = EKYItemType::None) const;
	TArray<TSharedPtr<FKYItemData>> GetEquippedWeapons() const;
	TArray<TSharedPtr<FKYItemData>> GetEquippedArmors() const;

	AKYPlayerState* GetPlayerState() const { return PlayerState.Get(); }

	// 소비 및 삭제
	bool UseItem(const FName& InstanceID);
	bool DropItem(const FName& InstanceID, int32 Count);
	bool DeleteItem(const FName& InstanceID); // 강제 삭제

	// 장착 관련
	bool EquipItem(const FName& InstanceID, uint8 Slot);
	bool UnequipItem(const FName& InstanceID, uint8 Slot);
	bool SwapWeaponSlot(uint8 From, uint8 To);

	// 추가/획득
	bool AddItem(const FName& BaseID, int32 Count);

	// 기타
	const FKYItemData* GetItemData(const FName& InstanceID) const;
	
private:
	TWeakObjectPtr<AKYPlayerState> PlayerState;
};