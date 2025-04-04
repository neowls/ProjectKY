// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerState.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "Struct/KYStruct.h"
#include "System/KYGameSingleton.h"

#define MAX_STACK_COUNT 99
#define MAX_WEAPON_SLOT 3


AKYPlayerState::AKYPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSetPlayer = CreateDefaultSubobject<UKYAttributeSetPlayer>(TEXT("AttributeSetPlayer"));

}

UAbilitySystemComponent* AKYPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

bool AKYPlayerState::AddItem(const FName& BaseItemID, int32 Count)
{
	// 데이터 테이블에서 기본 아이템 정보 가져오기
	FKYItemData* BaseItemData = GetBaseItemData(BaseItemID);
	if (!BaseItemData) return false;

	// 중첩 가능한 아이템이면 기존 인스턴스 검색하여 카운트 증가
	if (BaseItemData->ItemType == EKYItemType::Misc || BaseItemData->ItemType == EKYItemType::Usable)
	{
		for (auto& Pair : InventoryItems)
		{
			if (Pair.Value->InstanceData->BaseID == BaseItemID)
			{
				// 최대 스택 수 체크
				int32 NewCount = FMath::Min(Pair.Value->InstanceData->Count + Count, MAX_STACK_COUNT);
				int32 AddedCount = NewCount - Pair.Value->InstanceData->Count;
                
				if (AddedCount <= 0) return false; // 더 이상 스택 불가능
                
				Pair.Value->InstanceData->Count = NewCount;
				OnInventoryChanged.Broadcast(Pair.Key, true);
				return true;
			}
		}
	}

	// 새 인스턴스 ID 생성 (아이템ID_인스턴스번호 형식)
	FName InstanceID = GenerateInstanceID(BaseItemID);
    
	// 새 인스턴스 생성 (기본 아이템 데이터 복사)
	auto NewItemData = MakeShared<FKYItemData>(*BaseItemData);
	NewItemData->InstanceData = NewObject<UKYItem>(this, InstanceID);
	NewItemData->InstanceData->Count = Count;


	// 인벤토리에 추가
	InventoryItems.Add(InstanceID, NewItemData);
	InventoryWidgetDataArray.Add(NewItemData);
	
	OnInventoryChanged.Broadcast(InstanceID, true);
	return true;
}

bool AKYPlayerState::RemoveItem(const FName& InstanceID, int32 Count)
{
	auto ItemData = InventoryItems.FindRef(InstanceID);
	
	if (!ItemData.IsValid()) return false;

	// 장비 체크
	if (ItemData->ItemType > EKYItemType::Misc)
	{
		// 무기 들고 있으면 내려놓기
		if (ItemData->InstanceData->EquipState == EKYEquipmentState::InHand)
		{
			ToggleWeaponInHand(InstanceID);
		}
		
		// 장비 중이면 먼저 해제
		if (ItemData->InstanceData->EquipState == EKYEquipmentState::Equipped)
		{
			//UnequipItem(InstanceID);
		}
	}

	// 아이템 개수 감소
	ItemData->InstanceData->Count -= Count;
    
	// 개수가 0 이하면 완전히 제거
	if (ItemData->InstanceData->Count <= 0)
	{
		InventoryItems.Remove(InstanceID);
	}
    
	OnInventoryChanged.Broadcast(InstanceID, false);
	return true;
}

bool AKYPlayerState::UseItem(const FName& InstanceID)
{
	auto ItemData = InventoryItems.FindRef(InstanceID);
	if (!ItemData) return false;
	
	if (ItemData->ItemType == EKYItemType::Usable)
	{
		// 소비 아이템 사용
		ApplyItemEffects(InstanceID);
        
		// 개수 감소 및 업데이트
		ItemData->InstanceData->Count--;
		if (ItemData->InstanceData->Count <= 0)
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
	return InventoryItems.FindRef(InstanceID).Get();
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
		if (Pair.Value->ItemType == ItemType)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

FKYInventoryWidgetData AKYPlayerState::GetInventoryWidgetData(const FName& InstanceID)
{
	auto ItemData = InventoryItems.FindRef(InstanceID);
	if (!ItemData.IsValid())
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find Item Data : %s"), *InstanceID.ToString());
		return FKYInventoryWidgetData();
	}
	return *ItemData;
}

FKYInventoryWidgetData AKYPlayerState::GetEquippedItemWidgetData(const uint8 Index)
{
	auto ItemData = EquippedItems.FindRef(Index);
	if (!ItemData)
	{
		KY_LOG(LogKY, Warning, TEXT("Can't Find Item Data Index : %d"), Index);
		return FKYInventoryWidgetData();
	}
	return *ItemData;
}

TMap<uint8, FKYInventoryWidgetData> AKYPlayerState::GetEquippedItemsWidgetData()
{
	TMap<uint8, FKYInventoryWidgetData> Result;
	for (auto& Pair : EquippedItems)
	{
		Result.Add(Pair.Key, *Pair.Value);
	}

	return Result;
}

TArray<FKYInventoryWidgetData> AKYPlayerState::GetInventoryWidgetArrayData()
{
	TArray<FKYInventoryWidgetData> ValidItems;

	for (const TWeakPtr<FKYItemData>& WeakItem : InventoryWidgetDataArray)
	{
		if (TSharedPtr<FKYItemData> ItemData = WeakItem.Pin())
		{
			ValidItems.Add(FKYInventoryWidgetData(*ItemData));
		}
	}

	return ValidItems;
}

bool AKYPlayerState::EquipItem(const FName& InstanceID, uint8 TargetSlotIndex)
{
	if (!InventoryItems.Contains(InstanceID)) return false;
	
    auto NewItem = InventoryItems.FindRef(InstanceID);
	
	// 장비 아이템이 아닌 경우
    if (!NewItem.IsValid() || NewItem->ItemType < EKYItemType::Head) return false;

	// 장착하려는 칸에 다른 아이템이 있는지
	if (auto OldItem = EquippedItems.FindRef(TargetSlotIndex))
	{
		UnequipItem(OldItem->InstanceData.GetFName(), TargetSlotIndex);
	}
	
	// 방어구라면 효과 바로 적용 (무기의 경우 손에 들고 있어야 적용된다.)
	if(NewItem->ItemType != EKYItemType::Weapon) ApplyItemEffects(InstanceID);

	// 타겟 슬롯은 기본적으로 Enum Byte 기준으로 배정된다. 무기 타입의 경우 값이 8이지만 여분의 슬롯이 2개 더있으므로 (10-8) 해서 최대 2까지 남게된다.
	NewItem->InstanceData->AdditionalSlotIndex = TargetSlotIndex - static_cast<uint8>(NewItem->ItemType); 
	NewItem->InstanceData->EquipState = EKYEquipmentState::Equipped;
	
	EquippedItems.Add(TargetSlotIndex, NewItem);	// 장비에 추가
	InventoryItems.Remove(InstanceID);				// 인벤토리에서 제거
	
	OnEquipmentChanged.Broadcast(InstanceID, true); // UI 업데이트
	return true;
}

bool AKYPlayerState::UnequipItem(const FName& InstanceID, uint8 TargetSlotIndex)
{
	if (auto TargetItem = EquippedItems.FindRef(TargetSlotIndex))
	{
		RemoveItemEffects(InstanceID);
		
		TargetItem->InstanceData->AdditionalSlotIndex = -1;
		TargetItem->InstanceData->EquipState = EKYEquipmentState::Inventory;
		
		InventoryItems.Add(InstanceID, TargetItem);		// 인벤토리에 추가
		EquippedItems.Remove(TargetSlotIndex);			// 장비에서 제거
		
		OnEquipmentChanged.Broadcast(InstanceID, false); // UI 업데이트
		return true;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find Item : %s"), *InstanceID.ToString());
	return false;
}

bool AKYPlayerState::SwapWeaponSlot(uint8 FromSlotIndex, uint8 ToSlotIndex)
{
	// 동일 자리
	if (FromSlotIndex == ToSlotIndex) return false;

	auto TempFromWeapon = EquippedItems.FindRef(FromSlotIndex);
	auto TempToWeapon = EquippedItems.FindRef(ToSlotIndex);

	// 둘 다 nullptr 경우
	if (!TempFromWeapon.IsValid() && !TempToWeapon.IsValid()) return false;


	// 타겟 슬롯이 빈 슬롯일 경우 스왑이 아닌 옮겨진다.
	EquippedItems.FindOrAdd(FromSlotIndex) = TempToWeapon;
	EquippedItems.FindOrAdd(ToSlotIndex) = TempFromWeapon;

	return true;
}


bool AKYPlayerState::EquipWeapon(const FName& InstanceID, uint8 NewSlotIndex)
{
	auto NewWeapon = InventoryItems.FindRef(InstanceID);
	if (!NewWeapon.IsValid() || NewWeapon->ItemType != EKYItemType::Weapon) return false;

	if(EquippedWeapons.Contains(NewSlotIndex))
	{
		UnequipWeapon(NewSlotIndex);
	}
	
	NewWeapon->InstanceData->EquipState = EKYEquipmentState::Equipped;
	NewWeapon->InstanceData->AdditionalSlotIndex = NewSlotIndex - static_cast<uint8>(NewWeapon->ItemType);
	
	EquippedWeapons.Add(NewSlotIndex, NewWeapon);
	InventoryItems.Remove(InstanceID);
	
	OnEquipmentChanged.Broadcast(InstanceID, true);
	return true;
}

bool AKYPlayerState::EquipArmor(const FName& InstanceID)
{
	auto NewArmor = InventoryItems.FindRef(InstanceID);
	if(!NewArmor.IsValid() || NewArmor->ItemType < EKYItemType::Head) return false;

	//방어구 아이템 착용중인지
	if(auto OldArmor = EquippedArmors.FindRef(NewArmor->ItemType))
	{
		UnequipArmor(OldArmor->ItemType);
	}

	NewArmor->InstanceData->EquipState = EKYEquipmentState::Equipped;
	NewArmor->InstanceData->AdditionalSlotIndex = 0;
	
	EquippedArmors.Add(NewArmor->ItemType, NewArmor);
	InventoryItems.Remove(InstanceID);
	
	OnEquipmentChanged.Broadcast(InstanceID, true);
	return true;
}

bool AKYPlayerState::UnequipWeapon(uint8 TargetSlotIndex)
{
	if (auto TargetItem = EquippedWeapons.FindRef(TargetSlotIndex))
	{
		TargetItem->InstanceData->EquipState = EKYEquipmentState::Inventory;
		TargetItem->InstanceData->AdditionalSlotIndex = -1;
		
		InventoryItems.Add(TargetItem->InstanceData->GetFName(), TargetItem);
		EquippedWeapons.Remove(TargetSlotIndex);
		
		OnEquipmentChanged.Broadcast(TargetItem->InstanceData->GetFName(), false);
		return true;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find WeaponSlot : %d"), TargetSlotIndex);
	return false;
}

bool AKYPlayerState::UnequipArmor(EKYItemType TargetType)
{
	if (auto TargetItem = EquippedArmors.FindRef(TargetType))
	{
		TargetItem->InstanceData->EquipState = EKYEquipmentState::Inventory;
		TargetItem->InstanceData->AdditionalSlotIndex = -1;

		InventoryItems.Add(TargetItem->InstanceData->GetFName(), TargetItem);
		EquippedArmors.Remove(TargetType);
		
		OnEquipmentChanged.Broadcast(TargetItem->InstanceData->GetFName(), false);
		return true;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find Armor : %d"), TargetType);
	return false;
}



TArray<uint8> AKYPlayerState::GetEquippedItemIndex() const
{
	TArray<uint8> AllKeys;

	for (auto& Index: EquippedItems)
	{
		AllKeys.Add(Index.Key);
	}
	return AllKeys;
}


bool AKYPlayerState::ToggleWeaponInHand(const FName& InstanceID)
{
    auto ItemData = InventoryItems.FindRef(InstanceID);
    if (!ItemData || ItemData->ItemType != EKYItemType::Weapon) return false;
	
    // 장착되지 않았을 경우
    if (ItemData->InstanceData->EquipState != EKYEquipmentState::Equipped) return false;
    
    // 현재 무기를 들고 있는 경우
    if (TSharedPtr<FKYItemData> PreviousWeapon = GetCurrentInHandWeapon())
    {
    	PreviousWeapon->InstanceData->EquipState = EKYEquipmentState::Equipped;
    	OnWeaponStateChanged.Broadcast(PreviousWeapon->InstanceData->AdditionalSlotIndex, false);
    	
    	// 만약 동일한 무기라면 내리고 끝난다.
    	if (ItemData == PreviousWeapon)
    	{
    		SetCurrentInHandWeapon(nullptr);
    		return true;
    	}
    }
	
	// 다른 무기였다면 해당 무기를 들며 업데이트
	ItemData->InstanceData->EquipState = EKYEquipmentState::InHand;
	SetCurrentInHandWeapon(ItemData);
	OnWeaponStateChanged.Broadcast(ItemData->InstanceData->AdditionalSlotIndex, true);
	
    return true;
}


void AKYPlayerState::ApplyItemEffects(const FName& InstanceID)
{
	if (!ASC) return;
    
	auto ItemData = InventoryItems.FindRef(InstanceID);
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
		// 동일한 장비 아이템일 경우
		if (Pair.Value->InstanceData->BaseID == ItemID) NewInstanceIDIndex++;
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
