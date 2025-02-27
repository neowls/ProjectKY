// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KYCharacterWeaponAnimAsset.h"

#include "ProjectKY.h"

UKYCharacterWeaponAnimAsset::UKYCharacterWeaponAnimAsset()
{
	EventAnimMontageData.Add(FGameplayTag(), FEventAnimMontageData());
}

FPrimaryAssetId UKYCharacterWeaponAnimAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("KYWeaponData", GetFName());
}

FEventAnimMontageData UKYCharacterWeaponAnimAsset::GetEventAnimMontageData(FGameplayTag InGameplayTag)
{
	if (FEventAnimMontageData* FoundData = EventAnimMontageData.Find(InGameplayTag))
	{
		return *FoundData;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find Event Montage Data"));
	return FEventAnimMontageData();
	
}
