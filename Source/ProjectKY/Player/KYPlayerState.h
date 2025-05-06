// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Struct/KYStruct.h"
#include "KYPlayerState.generated.h"

class UKYItem;

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_MULTICAST_DELEGATE(FOnEquipmentChanged);

UCLASS()
class PROJECTKY_API AKYPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PreInitializeComponents() override;
	

	// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName&  BaseItemID, int32 Count = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FName& InstanceID, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(const FName& InstanceID);

	// 아이템 정보 조회
	const FKYItemData* GetItemData(const FName& InstanceID) const;

	static const TArray<EKYArmorSubType>& GetOrderedArmorSlots()
	{
		static const TArray<EKYArmorSubType> OrderedSlots = {
			EKYArmorSubType::Head,
			EKYArmorSubType::Chest,
			EKYArmorSubType::Hands,
			EKYArmorSubType::Legs,
			EKYArmorSubType::Feet
		};
		return OrderedSlots;
	}

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FName> GetInventoryItemKeys() const;

	const TMap<FName, TSharedPtr<FKYItemData>>& GetInventoryMap() const { return InventoryItems; }

	const TMap<EKYArmorSubType, TSharedPtr<FKYItemData>>& GetEquippedArmorMap() const { return EquippedArmors; }
	
	const TMap<uint8, TSharedPtr<FKYItemData>>& GetEquippedWeaponMap() const { return EquippedWeapons; }

	const TArray<TSharedPtr<FKYItemData>> GetInventoryArray() const;

	const TArray<TSharedPtr<FKYItemData>> GetEquippedArmorArray() const;

	const TArray<TSharedPtr<FKYItemData>> GetEquippedWeaponArray() const;
	

	
	// 장비 관리
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(const FName& InstanceID, uint8 TargetSlotIndex = 3);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipItem(const FName& InstanceID, uint8 TargetSlotIndex = 3);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool SwapWeaponSlot(uint8 FromSlotIndex, uint8 ToSlotIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipWeapon(const FName& InstanceID, uint8 NewSlotIndex = 0);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipArmor(const FName& InstanceID);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipWeapon(uint8 TargetSlotIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipArmor(EKYArmorSubType TargetType);
	

	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<uint8> GetEquippedItemIndex() const;
	
	
	// 무기 관리
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool ToggleWeaponInHand(uint8 SlotIndex);
	
	TSharedPtr<FKYItemData> GetCurrentInHandWeapon() const { return InHandWeapon; }

	void SetCurrentInHandWeapon(const TSharedPtr<FKYItemData>& NewWeapon);

	// 이벤트
	FOnInventoryChanged OnInventoryDataChanged;
	FOnInventoryChanged OnInventorySlotChanged;
	
	FOnEquipmentChanged OnEquipmentChanged;

	
	UPROPERTY(EditAnywhere, Category = "Debug")
	FTransform PreviewModelCamTransform;
	
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UKYAttributeSetPlayer> AttributeSetPlayer;



private:
	// 인벤토리 아이템 인스턴스
	TMap<FName, TSharedPtr<FKYItemData>> InventoryItems;

	TMap<uint8, TSharedPtr<FKYItemData>> EquippedItems;

	TMap<EKYArmorSubType, TSharedPtr<FKYItemData>> EquippedArmors;

	TMap<uint8, TSharedPtr<FKYItemData>> EquippedWeapons;

	TSharedPtr<FKYItemData> InHandWeapon;
	
	
	// GameplayEffect 핸들 관리
	UPROPERTY()
	TMap<FName, FActiveGameplayEffectHandle> EffectHandles;

	// 유틸리티 함수
	FName GenerateInstanceID();
	void ApplyItemEffects(const FName& InstanceID);
	void RemoveItemEffects(const FName& InstanceID);
	FKYItemData* GetBaseItemData(const FName& ItemID) const;
};


