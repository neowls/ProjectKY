// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/KYInventoryViewModel.h"

#include "ProjectKY.h"
#include "Player/KYPlayerState.h"

void UKYInventoryViewModel::Initialize(AKYPlayerState* InPlayerState)
{
	PlayerState = InPlayerState;
}

TArray<TSharedPtr<FKYItemData>> UKYInventoryViewModel::GetFilteredInventory(EKYItemType Category) const
{
	TArray<TSharedPtr<FKYItemData>> Result;
	if (!PlayerState.IsValid()) return Result;

	for (const auto& Item : PlayerState->GetInventoryArray())
	{
		if (Item.IsValid() && (Category == EKYItemType::None || Item->ItemType == Category))
		{
			Result.Add(Item);
		}
	}
	
	return Result;
}

TArray<TSharedPtr<FKYItemData>> UKYInventoryViewModel::GetEquippedWeapons() const
{
	TArray<TSharedPtr<FKYItemData>> Result;
	
	if (!PlayerState.IsValid()) return Result;
	return PlayerState->GetEquippedWeaponArray();
}

TArray<TSharedPtr<FKYItemData>> UKYInventoryViewModel::GetEquippedArmors() const
{
	TArray<TSharedPtr<FKYItemData>> Result;

	if (!PlayerState.IsValid()) return Result;
	return PlayerState->GetEquippedArmorArray();
}

bool UKYInventoryViewModel::UseItem(const FName& InstanceID)
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->UseItem(InstanceID);
}

bool UKYInventoryViewModel::DropItem(const FName& InstanceID, int32 Count)
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->RemoveItem(InstanceID, Count);
}

bool UKYInventoryViewModel::DeleteItem(const FName& InstanceID)
{
	return PlayerState->RemoveItem(InstanceID, MAX_int32);
}


bool UKYInventoryViewModel::EquipItem(const FName& InstanceID, uint8 Slot)
{
	if (!PlayerState.IsValid()) return false;
	KY_LOG(LogKY, Warning, TEXT("Equip %s Item"), *InstanceID.ToString());
	return PlayerState->EquipItem(InstanceID, Slot);;
}

bool UKYInventoryViewModel::UnequipItem(const FName& InstanceID, uint8 Slot)
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->UnequipItem(InstanceID, Slot);;
}

bool UKYInventoryViewModel::SwapWeaponSlot(uint8 From, uint8 To)
{
	return PlayerState.IsValid() && PlayerState->SwapWeaponSlot(From, To);
}

bool UKYInventoryViewModel::AddItem(const FName& BaseID, int32 Count)
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->AddItem(BaseID, Count);
}

const FKYItemData* UKYInventoryViewModel::GetItemData(const FName& InstanceID) const
{
	if (!PlayerState.IsValid()) return nullptr;
	return PlayerState->GetItemData(InstanceID);
}
