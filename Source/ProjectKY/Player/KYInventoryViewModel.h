#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Struct/KYStruct.h"
#include "KYInventoryViewModel.generated.h"

class UKYInventoryItemObject;
class AKYPlayerState;

DECLARE_DYNAMIC_DELEGATE(FOnItemSlotChanged);

UCLASS()
class PROJECTKY_API UKYInventoryViewModel : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(AKYPlayerState* InPlayerState);

	const TArray<TObjectPtr<UKYInventoryItemObject>>& GetVisibleInventory() const { return VisibleInventory; }
	const TArray<TObjectPtr<UKYInventoryItemObject>>& GetVisibleEquippedWeapons() const { return VisibleWeapons; }
	const TArray<TObjectPtr<UKYInventoryItemObject>>& GetVisibleEquippedArmors() const { return VisibleArmors; }

	void InitializeEquipmentWrappers();
	void RefreshInventoryWrapperList();
	
	void SetFilterCategory(EKYItemType Category);
	
	void FlushAllItemChanges();
	void FlushInventoryChanges();
	void FlushEquipmentChanges();


	AKYPlayerState* GetPlayerState() const { return PlayerState.Get(); }

	EKYItemType GetCurrentCategory() const { return CurrentFilterCategory; }
	
	FOnItemSlotChanged OnItemSlotChanged;

	// 소비 및 삭제
	bool UseItem(const FName& InstanceID);
	bool DropItem(const FName& InstanceID, int32 Count);
	bool DeleteItem(const FName& InstanceID); // 강제 삭제

	// 장착 관련
	bool EquipItem(const FName& InstanceID, uint8 Slot = 0) const;
	bool UnequipItem(const FName& InstanceID, uint8 Slot, const EKYItemType& ItemType);
	bool SwapWeaponSlot(uint8 From, uint8 To);

	// 추가/획득
	bool AddItem(const FName& BaseID, int32 Count);

	// 기타
	const FKYItemData* GetItemData(const FName& InstanceID) const;
	
private:
	UPROPERTY()
	TWeakObjectPtr<AKYPlayerState> PlayerState;

	// ViewModel 내부 구조
	UPROPERTY()
	TMap<FName, TObjectPtr<UKYInventoryItemObject>> WrapperPool;

	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleInventory;

	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleWeapons;

	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleArmors;

	// 선택 필터 상태
	EKYItemType CurrentFilterCategory = EKYItemType::None;
};