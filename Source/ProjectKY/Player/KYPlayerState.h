// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Struct/KYStruct.h"
#include "KYPlayerState.generated.h"

class UKYItem;
class UKYEquipmentItem;
class UKYWeaponItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, FName, InstanceID, bool, bAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EKYEquipmentType, EquipmentType, bool, bEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponStateChanged, int32, SlotIndex, bool, bInHand);

UCLASS()
class PROJECTKY_API AKYPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& ItemID, int32 Count = 1);
    
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FName& InstanceID, int32 Count = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(const FName& InstanceID);

	// 아이템 정보 조회
	const FKYItemData* GetItemData(const FName& InstanceID) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FName> GetInventoryItems() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FName> GetItemsByType(EKYItemType ItemType) const;
	
	// 장비 관리
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(const FName& InstanceID);
    
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipItem(const FName& InstanceID);

	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<FName> GetEquippedItems() const;
    
	UFUNCTION(BlueprintPure, Category = "Equipment")
	FName GetEquippedItemByType(EKYEquipmentType EquipmentType) const;
	
	// 무기 관리
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool ToggleWeaponInHand(const FName& InstanceID);
	
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FName GetCurrentInHandWeapon() const;

	// 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
    
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged OnEquipmentChanged;
    
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStateChanged OnWeaponStateChanged;
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UKYAttributeSetPlayer> AttributeSetPlayer;
	

private:
	// 인벤토리 아이템 인스턴스
	UPROPERTY()
	TMap<FName, FKYItemData> InventoryItems;

	// 마지막으로 생성된 인스턴스 ID 추적
	UPROPERTY()
	int32 LastInstanceIndex;
	
	// GameplayEffect 핸들 관리
	UPROPERTY()
	TMap<FName, FActiveGameplayEffectHandle> EffectHandles;

	// 유틸리티 함수
	FName GenerateInstanceID(const FName& ItemID);
	void ApplyItemEffects(const FName& InstanceID);
	void RemoveItemEffects(const FName& InstanceID);
	FKYItemData* GetBaseItemData(const FName& ItemID) const;
};

