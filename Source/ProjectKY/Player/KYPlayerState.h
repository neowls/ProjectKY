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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, const FName&, InstanceID, bool, bAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, const FName&, InstanceID, bool, bEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponStateChanged, uint8, SlotIndex, bool, bInHand);

UCLASS()
class PROJECTKY_API AKYPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYPlayerState();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 인벤토리
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FName& BaseItemID, int32 Count = 1);
    
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

	FKYInventoryWidgetData GetInventoryWidgetData(const FName& InstanceID);

	UFUNCTION(Blueprintable, Category = "Inventory")
	TArray<FKYInventoryWidgetData> GetInventoryWidgetArrayData();

	
	// 장비 관리
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipItem(const FName& InstanceID, uint8 TargetSlotIndex = 3);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipItem(const FName& InstanceID, uint8 TargetSlotIndex = 3);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool SwapWeaponSlot(uint8 FromSlotIndex, uint8 ToSlotIndex);

	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipWeapon(const FName& InstanceID, uint8 NewSlotIndex = 7);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool EquipArmor(const FName& InstanceID);
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipWeapon(uint8 TargetSlotIndex = 7);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
	bool UnequipArmor(EKYItemType TargetType);
	

	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<uint8> GetEquippedItemIndex() const;

	FKYInventoryWidgetData GetEquippedItemWidgetData(const uint8 Index);

	TMap<uint8, FKYInventoryWidgetData> GetEquippedItemsWidgetData();
	
	// 무기 관리
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool ToggleWeaponInHand(const FName& InstanceID);
	
	TSharedPtr<FKYItemData> GetCurrentInHandWeapon() const { return InHandWeapon; }

	void SetCurrentInHandWeapon(TSharedPtr<FKYItemData> NewWeapon) { InHandWeapon = NewWeapon; };

	// 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
    
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged OnEquipmentChanged;
    
	UPROPERTY(BlueprintAssignable)
	FOnWeaponStateChanged OnWeaponStateChanged;

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

	TArray<TWeakPtr<FKYItemData>> InventoryWidgetDataArray;

	TMap<uint8, TSharedPtr<FKYItemData>> EquippedItems;

	TMap<EKYItemType, TSharedPtr<FKYItemData>> EquippedArmors;

	TMap<uint8, TSharedPtr<FKYItemData>> EquippedWeapons;

	TSharedPtr<FKYItemData> InHandWeapon;

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

