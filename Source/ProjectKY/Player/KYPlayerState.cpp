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

void AKYPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	for (auto Slot : GetOrderedArmorSlots())
	{
		FKYItemData InitialSlotData;
		InitialSlotData.ItemType = EKYItemType::Armor;
		InitialSlotData.ArmorType = Slot;
		EquippedArmors.Add(Slot, MakeShared<FKYItemData>(InitialSlotData));
	}

	for (int i = 0; i <3; i++)
	{
		FKYItemData InitialSlotData;
		InitialSlotData.ItemType = EKYItemType::Weapon;
		EquippedWeapons.Add(i, MakeShared<FKYItemData>(InitialSlotData));
	}
		
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
			if (Pair.Value->InstanceData.BaseID == BaseItemID)
			{
				// 최대 스택 수 체크
				int32 NewCount = FMath::Min(Pair.Value->InstanceData.Count + Count, MAX_STACK_COUNT);
				int32 AddedCount = NewCount - Pair.Value->InstanceData.Count;
                
				if (AddedCount <= 0) return false; // 더 이상 스택 불가능
                
				Pair.Value->InstanceData.Count = NewCount;
				OnInventoryDataChanged.Broadcast();
				return true;
			}
		}
	}
	// 중첩 불가능한 아이템이면 새로 추가

	// 새 인스턴스 ID 생성 (GUID 형식)
	FName InstanceID = GenerateInstanceID();
    
	// 새 인스턴스 생성 (기본 아이템 데이터 복사)
	auto NewItemData = MakeShared<FKYItemData>(*BaseItemData);
	NewItemData->InstanceData.BaseID = BaseItemID;
	NewItemData->InstanceData.InstanceID = InstanceID;
	NewItemData->InstanceData.Count = Count;
	NewItemData->InstanceData.EquipState = EKYEquipmentState::Inventory;

	// 인벤토리에 추가
	InventoryItems.Add(InstanceID, NewItemData);
	OnInventorySlotChanged.Broadcast();
	
	return true;
}


bool AKYPlayerState::RemoveItem(const FName& InstanceID, int32 Count)
{
	auto ItemData = InventoryItems.FindRef(InstanceID);
	
	if (!ItemData.IsValid()) return false;

	Count = FMath::Clamp(Count, 1, ItemData->InstanceData.Count);

	// 아이템 개수 감소
	ItemData->InstanceData.Count -= Count;
    
	// 개수가 0 이하면 완전히 제거
	if (ItemData->InstanceData.Count <= 0)
	{
		InventoryItems.Remove(InstanceID);
		OnInventorySlotChanged.Broadcast();
		return true;
	}

	OnInventoryDataChanged.Broadcast();
	return true;
}

bool AKYPlayerState::UseItem(const FName& InstanceID)
{
	auto ItemData = InventoryItems.FindRef(InstanceID);
	if (!ItemData) return false;
	
	if (ItemData->ItemType == EKYItemType::Usable)
	{
		// 개수 감소 및 업데이트
		if (RemoveItem(InstanceID))
		{
			// 소비 아이템 사용
			ApplyItemEffects(InstanceID);
			return true;
		}
		return false;
	}
    
	return false;
}

const FKYItemData* AKYPlayerState::GetItemData(const FName& InstanceID) const
{
	return InventoryItems.FindRef(InstanceID).Get();
}

TArray<FName> AKYPlayerState::GetInventoryItemKeys() const
{
    TArray<FName> Result;
    InventoryItems.GetKeys(Result);
    return Result;
}

const TArray<TSharedPtr<FKYItemData>> AKYPlayerState::GetInventoryArray() const
{
	TArray<TSharedPtr<FKYItemData>> Result;
	InventoryItems.GenerateValueArray(Result);
	return Result;
}

const TArray<TSharedPtr<FKYItemData>> AKYPlayerState::GetEquippedArmorArray() const
{
	TArray<TSharedPtr<FKYItemData>> Result;
	EquippedArmors.GenerateValueArray(Result);
	return Result;
}

const TArray<TSharedPtr<FKYItemData>> AKYPlayerState::GetEquippedWeaponArray() const
{
	TArray<TSharedPtr<FKYItemData>> Result;
	EquippedWeapons.GenerateValueArray(Result);
	return Result;
}

bool AKYPlayerState::EquipItem(const FName& InstanceID, uint8 TargetSlotIndex)
{
	if (!InventoryItems.Contains(InstanceID)) return false;
	
    auto NewItem = InventoryItems.FindRef(InstanceID);
	
	// 장비 아이템이 아닌 경우
    if (NewItem->ItemType != EKYItemType::Armor && NewItem->ItemType != EKYItemType::Weapon) return false;

	KY_LOG(LogKY, Warning, TEXT("Pass Equipment Check."));
	
	// 장착하려는 칸에 다른 아이템이 있는지
	if (auto OldItem = EquippedItems.FindRef(TargetSlotIndex))
	{
		UnequipItem(OldItem->InstanceData.InstanceID, TargetSlotIndex);
	}
	
	// 방어구라면 효과 바로 적용 (무기의 경우 손에 들고 있어야 적용된다.)
	if(NewItem->ItemType != EKYItemType::Weapon) ApplyItemEffects(InstanceID);

	// 타겟 슬롯은 기본적으로 Enum Byte 기준으로 배정된다. 무기 타입의 경우 값이 8이지만 여분의 슬롯이 2개 더있으므로 (10-8) 해서 최대 2까지 남게된다.
	NewItem->InstanceData.AdditionalSlotIndex = TargetSlotIndex; 
	NewItem->InstanceData.EquipState = EKYEquipmentState::Equipped;
	

	if (NewItem->ItemType == EKYItemType::Weapon)
	{	
		EquippedWeapons[TargetSlotIndex]->SetData(*NewItem);
	}
	else
	{
		EquippedArmors[NewItem->ArmorType]->SetData(*NewItem);
	}
	
	RemoveItem(InstanceID);
	
	OnEquipmentChanged.Broadcast(); // UI 업데이트

	return true;
}

bool AKYPlayerState::UnequipItem(const FName& InstanceID, uint8 TargetSlotIndex, const EKYItemType& ItemType)
{
	TSharedPtr<FKYItemData> NewItem = ItemType == EKYItemType::Weapon ?
	EquippedWeapons[TargetSlotIndex] : EquippedArmors[static_cast<EKYArmorSubType>(TargetSlotIndex)];
	if (NewItem.IsValid())
	{
		RemoveItemEffects(InstanceID);
		
		NewItem->InstanceData.AdditionalSlotIndex = 255;
		NewItem->InstanceData.EquipState = EKYEquipmentState::Inventory;

		if (ItemType == EKYItemType::Weapon)
		{
			EquippedWeapons[TargetSlotIndex]->ClearData();
		}
		else
		{
			EquippedArmors[NewItem->ArmorType]->ClearData();
		}
		
		InventoryItems.Add(InstanceID, NewItem);

		OnEquipmentChanged.Broadcast(); // UI 업데이트
		
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
	
	UnequipWeapon(NewSlotIndex);

	NewWeapon->InstanceData.EquipState = EKYEquipmentState::Equipped;
	NewWeapon->InstanceData.AdditionalSlotIndex = NewSlotIndex;

	*EquippedWeapons[NewSlotIndex] = *NewWeapon;
	RemoveItem(InstanceID);
	
	OnEquipmentChanged.Broadcast();
	return true;
}

bool AKYPlayerState::EquipArmor(const FName& InstanceID)
{
	auto NewArmor = InventoryItems.FindRef(InstanceID);
	if(!NewArmor.IsValid() || NewArmor->ItemType != EKYItemType::Armor) return false;

	UnequipArmor(NewArmor->ArmorType);

	NewArmor->InstanceData.EquipState = EKYEquipmentState::Equipped;
	
	*EquippedArmors[NewArmor->ArmorType] = *NewArmor;
	RemoveItem(InstanceID);


	OnEquipmentChanged.Broadcast();
	return true;
}

bool AKYPlayerState::UnequipWeapon(uint8 TargetSlotIndex)
{
	if (auto TargetItem = EquippedWeapons.FindRef(TargetSlotIndex); !TargetItem->IsEmpty())
	{
		TargetItem->InstanceData.EquipState = EKYEquipmentState::Inventory;
		TargetItem->InstanceData.AdditionalSlotIndex = 255;
		
		InventoryItems.Add(TargetItem->InstanceData.InstanceID, TargetItem);
		EquippedWeapons[TargetSlotIndex]->ClearData();
		
		OnEquipmentChanged.Broadcast();
		return true;
	}

	KY_LOG(LogKY, Warning, TEXT("WeaponSlot : %d Is Empty"), TargetSlotIndex);
	return false;
}

bool AKYPlayerState::UnequipArmor(EKYArmorSubType TargetType)
{
	if (auto TargetItem = EquippedArmors.FindRef(TargetType); !TargetItem->IsEmpty())
	{
		TargetItem->InstanceData.EquipState = EKYEquipmentState::Inventory;

		InventoryItems.Add(TargetItem->InstanceData.InstanceID, TargetItem);
		EquippedArmors[TargetType]->ClearData();
		
		OnEquipmentChanged.Broadcast();
		return true;
	}

	KY_LOG(LogKY, Warning, TEXT("ArmorSlot : %d Is Empty"), TargetType);
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


bool AKYPlayerState::ToggleWeaponInHand(uint8 SlotIndex)
{
    const auto ItemData = EquippedWeapons.FindRef(SlotIndex);
    if (!ItemData || ItemData->ItemType != EKYItemType::Weapon) return false;
	
    // 장착되지 않았을 경우
    if (ItemData->InstanceData.EquipState != EKYEquipmentState::Equipped) return false;
    
    // 현재 무기를 들고 있는 경우
    if (TSharedPtr<FKYItemData> PreviousWeapon = GetCurrentInHandWeapon())
    {
    	PreviousWeapon->InstanceData.EquipState = EKYEquipmentState::Equipped;
    	SetCurrentInHandWeapon(nullptr);
    	// 만약 동일한 무기라면 내리고 끝난다.
    	if (ItemData == PreviousWeapon)
    	{
    		OnEquipmentChanged.Broadcast();
    		return true;
    	}
    }
	
	// 다른 무기였다면 해당 무기를 들며 업데이트
	ItemData->InstanceData.EquipState = EKYEquipmentState::InHand;
	SetCurrentInHandWeapon(ItemData);
	OnEquipmentChanged.Broadcast();
    return true;
}

void AKYPlayerState::SetCurrentInHandWeapon(const TSharedPtr<FKYItemData>& NewWeapon)
{
	if (NewWeapon)
	{
		ApplyItemEffects(NewWeapon->InstanceData.InstanceID);
	}
	else
	{
		RemoveItemEffects(InHandWeapon->InstanceData.InstanceID);
	}
	InHandWeapon = NewWeapon;
}


void AKYPlayerState::ApplyItemEffects(const FName& InstanceID)
{
	if (!ASC) return;
    
	auto ItemData = InventoryItems.FindRef(InstanceID);
	if (!ItemData || !ItemData->EffectClass) return;
    
	// 이미 적용된 효과가 있으면 제거
	if (EffectHandles.Contains(InstanceID)) RemoveItemEffects(InstanceID);
    
	// 새 효과 적용
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);
    
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ItemData->EffectClass, 1, EffectContext);
    
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


FName AKYPlayerState::GenerateInstanceID()
{
	FGuid NewGUID = FGuid::NewGuid();
	return FName(*NewGUID.ToString());
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
