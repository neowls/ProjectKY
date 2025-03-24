// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "Item/KYItem.h"
#include "Struct/KYStruct.h"
#include "System/KYGameSingleton.h"

AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetPlayer = CreateDefaultSubobject<UKYAttributeSetPlayer>(TEXT("AttributeSetPlayer"));
}

UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

bool AKYPlayerState::AddItem(const FName& ItemID, int32 Count)
{
	// 데이터 테이블에서 기본 아이템 정보 가져오기
	const FKYItemData* BaseItemData = GetBaseItemData(ItemID);
	if (!BaseItemData) return false;

	// 중첩 가능한 아이템이면 기존 인스턴스 검색하여 카운트 증가
	if (BaseItemData->bIsStackable)
	{
		for (auto& Pair : InventoryItems)
		{
			if (Pair.Value.ItemID == ItemID)
			{
				// 최대 스택 수 체크
				int32 NewCount = FMath::Min(Pair.Value.Count + Count, BaseItemData->MaxStackCount);
				int32 AddedCount = NewCount - Pair.Value.Count;
                
				if (AddedCount <= 0) return false; // 더 이상 스택 불가능
                
				Pair.Value.Count = NewCount;
				OnInventoryChanged.Broadcast(Pair.Key, true);
				return true;
			}
		}
	}

	// 새 인스턴스 ID 생성 (아이템ID_인스턴스번호 형식)
	FName InstanceID = GenerateInstanceID(ItemID);
    
	// 새 인스턴스 생성 (기본 아이템 데이터 복사)
	FKYItemData NewItemData = *BaseItemData;
	NewItemData.Count = Count;
    
	// 인벤토리에 추가
	InventoryItems.Add(InstanceID, NewItemData);
    
	OnInventoryChanged.Broadcast(InstanceID, true);
	return true;
}

bool AKYPlayerState::RemoveItem(const FName& InstanceID, int32 Count)
{
	FKYItemData* ItemData = InventoryItems.Find(InstanceID);
	if (!ItemData) return false;

	// 장비 SubData 체크
	UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
	if (EquipItem)
	{
		// 장비 중이면 먼저 해제
		if (EquipItem->bIsEquipped)
		{
			UnequipItem(InstanceID);
		}
        
		// 무기 들고 있으면 내려놓기
		UKYWeaponItem* WeaponItem = Cast<UKYWeaponItem>(EquipItem);
		if (WeaponItem && WeaponItem->bIsInHand)
		{
			ToggleWeaponInHand(InstanceID);
		}
	}

	// 아이템 개수 감소
	ItemData->Count -= Count;
    
	// 개수가 0 이하면 완전히 제거
	if (ItemData->Count <= 0)
	{
		InventoryItems.Remove(InstanceID);
	}
    
	OnInventoryChanged.Broadcast(InstanceID, false);
	return true;
}

bool AKYPlayerState::UseItem(const FName& InstanceID)
{
	FKYItemData* ItemData = InventoryItems.Find(InstanceID);
	if (!ItemData) return false;

	if (ItemData->ItemType == EKYItemType::Equipment)
	{
		// 장비 아이템은 장착 상태 토글
		UKYEquipmentItem* EquipmentItem = Cast<UKYEquipmentItem>(ItemData->SubData);
		if (!EquipmentItem) return false;
        
		if (EquipmentItem->bIsEquipped)
		{
			return UnequipItem(InstanceID);
		}
		return EquipItem(InstanceID);
		
	}
	if (ItemData->ItemType == EKYItemType::Usable)
	{
		// 소비 아이템 사용
		UKYConsumableItem* ConsumableItem = Cast<UKYConsumableItem>(ItemData->SubData);
		if (!ConsumableItem) return false;
        
		// 효과 적용
		ApplyItemEffects(InstanceID);
        
		// 개수 감소 및 업데이트
		ItemData->Count--;
		if (ItemData->Count <= 0)
		{
			InventoryItems.Remove(InstanceID);
		}
        
		OnInventoryChanged.Broadcast(InstanceID, false);
		return true;
	}
    
	return false;
}

const FKYItemData* AKYPlayerState::GetItemData(const FName& InstanceID) const
{
	return InventoryItems.Find(InstanceID);
}

TArray<FName> AKYPlayerState::GetInventoryItems() const
{
    TArray<FName> Result;
    InventoryItems.GetKeys(Result);
    return Result;
}

TArray<FName> AKYPlayerState::GetItemsByType(EKYItemType ItemType) const
{
	TArray<FName> Result;
	for (auto& Pair : InventoryItems)
	{
		if (Pair.Value.ItemType == ItemType)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

bool AKYPlayerState::EquipItem(const FName& InstanceID)
{
    FKYItemData* ItemData = InventoryItems.Find(InstanceID);
    if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) return false;
    
    UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
    if (!EquipItem) return false;

    // 이미 장착 중이면 무시
    if (EquipItem->bIsEquipped) return true;
    
    // 같은 타입의 장비 해제
    for (auto& Pair : InventoryItems)
    {
        if (Pair.Key != InstanceID)
        {
            FKYItemData* OtherItemData = &Pair.Value;
            UKYEquipmentItem* OtherEquip = Cast<UKYEquipmentItem>(OtherItemData->SubData);
            
            if (OtherEquip && OtherEquip->bIsEquipped && OtherEquip->EquipmentType == EquipItem->EquipmentType)
            {
                UnequipItem(Pair.Key);
            }
        }
    }
    
    // 장비 장착 및 효과 적용
    EquipItem->bIsEquipped = true;
    ApplyItemEffects(InstanceID);
    OnEquipmentChanged.Broadcast(EquipItem->EquipmentType, true);
    
    return true;
}

bool AKYPlayerState::UnequipItem(const FName& InstanceID)
{
    FKYItemData* ItemData = InventoryItems.Find(InstanceID);
    if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) return false;
    
    UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
    if (!EquipItem || !EquipItem->bIsEquipped) return false;
    
    // 무기인 경우 먼저 손에서 내리기
    if (EquipItem->EquipmentType == EKYEquipmentType::Weapon)
    {
        UKYWeaponItem* WeaponItem = Cast<UKYWeaponItem>(EquipItem);
        if (WeaponItem && WeaponItem->bIsInHand)
        {
            ToggleWeaponInHand(InstanceID);
        }
    }
    
    // 장비 해제
    EquipItem->bIsEquipped = false;
    RemoveItemEffects(InstanceID);
    OnEquipmentChanged.Broadcast(EquipItem->EquipmentType, false);
    
    return true;
}

TArray<FName> AKYPlayerState::GetEquippedItems() const
{
    TArray<FName> Result;
    
    for (const auto& Pair : InventoryItems)
    {
        FKYItemData ItemData = Pair.Value;
        if (ItemData.ItemType == EKYItemType::Equipment)
        {
            UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData.SubData);
            if (EquipItem && EquipItem->bIsEquipped)
            {
                Result.Add(Pair.Key);
            }
        }
    }
    
    return Result;
}

FName AKYPlayerState::GetEquippedItemByType(EKYEquipmentType EquipmentType) const
{
    for (const auto& Pair : InventoryItems)
    {
        FKYItemData ItemData = Pair.Value;
        if (ItemData.ItemType == EKYItemType::Equipment)
        {
            UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData.SubData);
            if (EquipItem && EquipItem->bIsEquipped && EquipItem->EquipmentType == EquipmentType)
            {
                return Pair.Key;
            }
        }
    }
    
    return NAME_None;
}

bool AKYPlayerState::ToggleWeaponInHand(const FName& InstanceID)
{
    FKYItemData* ItemData = InventoryItems.Find(InstanceID);
    if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) return false;
    
    UKYEquipmentItem* EquipmentItem = Cast<UKYEquipmentItem>(ItemData->SubData);
    if (!EquipmentItem || EquipmentItem->EquipmentType != EKYEquipmentType::Weapon) return false;
    
    UKYWeaponItem* WeaponItem = Cast<UKYWeaponItem>(EquipmentItem);
    if (!WeaponItem) return false;
    
    // 장착되지 않았으면 먼저 장착
    if (!WeaponItem->bIsEquipped)
    {
        if (!EquipItem(InstanceID)) return false;
    }
    
    // 현재 손에 들고 있는 무기 처리
    if (!WeaponItem->bIsInHand)
    {
        // 다른 무기를 들고 있으면 먼저 내리기
        for (auto& Pair : InventoryItems)
        {
            if (Pair.Key != InstanceID)
            {
                FKYItemData* OtherItemData = &Pair.Value;
                if (OtherItemData->ItemType == EKYItemType::Equipment)
                {
                    UKYEquipmentItem* OtherEquip = Cast<UKYEquipmentItem>(OtherItemData->SubData);
                    if (OtherEquip && OtherEquip->EquipmentType == EKYEquipmentType::Weapon)
                    {
                        UKYWeaponItem* OtherWeapon = Cast<UKYWeaponItem>(OtherEquip);
                        if (OtherWeapon && OtherWeapon->bIsInHand)
                        {
                            OtherWeapon->bIsInHand = false;
                            RemoveItemEffects(Pair.Key);
                            OnWeaponStateChanged.Broadcast(OtherWeapon->WeaponSlotIndex, false);
                        }
                    }
                }
            }
        }
        
        // 무기 들기
        WeaponItem->bIsInHand = true;
        ApplyItemEffects(InstanceID);
    }
    else
    {
        // 무기 내리기
        WeaponItem->bIsInHand = false;
        RemoveItemEffects(InstanceID);
    }
    
    OnWeaponStateChanged.Broadcast(WeaponItem->WeaponSlotIndex, WeaponItem->bIsInHand);
    return true;
}

FName AKYPlayerState::GetCurrentInHandWeapon() const
{
    for (const auto& Pair : InventoryItems)
    {
        FKYItemData ItemData = Pair.Value;
        if (ItemData.ItemType == EKYItemType::Equipment)
        {
            UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData.SubData);
            if (EquipItem && EquipItem->EquipmentType == EKYEquipmentType::Weapon)
            {
                UKYWeaponItem* WeaponItem = Cast<UKYWeaponItem>(EquipItem);
                if (WeaponItem && WeaponItem->bIsInHand)
                {
                    return Pair.Key;
                }
            }
        }
    }
    
    return NAME_None;
}


void AKYPlayerState::ApplyItemEffects(const FName& InstanceID)
{
	if (!ASC) return;
    
	FKYItemData* ItemData = InventoryItems.Find(InstanceID);
	if (!ItemData || !ItemData->EffectClass) return;
    
	// 이미 적용된 효과가 있으면 제거
	RemoveItemEffects(InstanceID);
    
	// 새 효과 적용
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
    
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		ItemData->EffectClass, 1, EffectContext);
    
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (Handle.IsValid())
		{
			EffectHandles.Add(InstanceID, Handle);
		}
	}
}

FKYItemData* AKYPlayerState::GetBaseItemData(const FName& ItemID) const
{
	return UKYGameSingleton::Get().GetItemDataByRowName(ItemID);
}


FName AKYPlayerState::GenerateInstanceID(const FName& ItemID)
{
	int32 NewInstanceIDIndex = 0;
	for (auto& Pair : InventoryItems)
	{
		if (Pair.Value.ItemID == ItemID) NewInstanceIDIndex++;
	}
	
	return FName(*FString::Printf(TEXT("%s_%d"), *ItemID.ToString(), NewInstanceIDIndex));
}


void AKYPlayerState::RemoveItemEffects(const FName& InstanceID)
{
	if (!ASC) return;
    
	FActiveGameplayEffectHandle* Handle = EffectHandles.Find(InstanceID);
	if (Handle && Handle->IsValid())
	{
		ASC->RemoveActiveGameplayEffect(*Handle);
		EffectHandles.Remove(InstanceID);
	}
}
