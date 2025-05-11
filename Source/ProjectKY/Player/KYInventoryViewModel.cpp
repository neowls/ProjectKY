// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/KYInventoryViewModel.h"

#include "ProjectKY.h"
#include "Data/KYInventoryItemObject.h"
#include "Player/KYPlayerState.h"

void UKYInventoryViewModel::Initialize(AKYPlayerState* InPlayerState)
{
	PlayerState = InPlayerState;

	if (!PlayerState.IsValid()) return;
	
	PlayerState->OnInventorySlotChanged.AddUObject(this, &ThisClass::RefreshInventoryWrapperList);
	PlayerState->OnInventoryDataChanged.AddUObject(this, &ThisClass::FlushInventoryChanges);
	PlayerState->OnEquipmentChanged.AddUObject(this, &ThisClass::FlushEquipmentChanges);
	
	RefreshInventoryWrapperList();
	InitializeEquipmentWrappers();
}

void UKYInventoryViewModel::SetFilterCategory(EKYItemType Category)
{
	CurrentFilterCategory = Category;
	RefreshInventoryWrapperList();
}

void UKYInventoryViewModel::FlushAllItemChanges()
{
	FlushInventoryChanges();
	FlushEquipmentChanges();
}

void UKYInventoryViewModel::FlushInventoryChanges()
{
	for (auto& Wrapper : VisibleInventory)
	{
		if (!Wrapper->IsEmpty() && Wrapper->IsValidLowLevel() && Wrapper->GetItemData().InstanceData.IsDirty())
		{
			Wrapper->HandleDataChanged();
		}
	}
}

void UKYInventoryViewModel::FlushEquipmentChanges()
{
	for (auto& Wrapper : VisibleWeapons)
	{
		if (!Wrapper->IsEmpty() && Wrapper->GetItemData().InstanceData.IsDirty())
		{
			Wrapper->HandleDataChanged();
		}
	}

	for (auto& Wrapper : VisibleArmors)
	{
		if (!Wrapper->IsEmpty() && Wrapper->GetItemData().InstanceData.IsDirty())
		{
			Wrapper->HandleDataChanged();
		}
	}
}

void UKYInventoryViewModel::InitializeEquipmentWrappers()
{
	VisibleWeapons.Reset();
	VisibleArmors.Reset();

	// 무기 슬롯
	for (auto Data : PlayerState->GetEquippedWeaponArray())
	{
		UKYInventoryItemObject* Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->SetData(Data);
		VisibleWeapons.Add(Wrapper);
	}

	// 방어구 슬롯
	for (auto Data : PlayerState->GetEquippedArmorArray())
	{
		UKYInventoryItemObject* Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->SetData(Data);
		VisibleArmors.Add(Wrapper);
	}
}

void UKYInventoryViewModel::RefreshInventoryWrapperList()
{
	VisibleInventory.Reset();
	if (PlayerState->GetInventoryArray().Num() == 0) return;

	for (auto& Item : PlayerState->GetInventoryArray())
	{
		if (!Item.IsValid()) continue;
		
		const FName& ID = Item->InstanceData.InstanceID;
		UKYInventoryItemObject* Wrapper = WrapperPool.Contains(ID) ? WrapperPool.FindRef(ID) : *NewObject<UKYInventoryItemObject>(this);
		
		if (!WrapperPool.Contains(ID))
		{
			Wrapper->SetData(Item);
			WrapperPool.Add(ID, Wrapper);
		}

		Wrapper->OnChanged.Clear();
		
		if (CurrentFilterCategory == EKYItemType::None || Item->ItemType == CurrentFilterCategory)
		{
			VisibleInventory.Add(Wrapper);
		}
	}
	
	OnItemSlotChanged.ExecuteIfBound();
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


bool UKYInventoryViewModel::EquipItem(const FName& InstanceID, uint8 Slot) const
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->EquipItem(InstanceID, Slot);;
}

bool UKYInventoryViewModel::UnequipItem(const FName& InstanceID, uint8 Slot, const EKYItemType& ItemType)
{
	if (!PlayerState.IsValid()) return false;
	return PlayerState->UnequipItem(InstanceID, Slot, ItemType);
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
