// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KYCharacterBaseAsset.h"

FEventAnimMontageData UKYCharacterBaseAsset::GetEventAnimMontageData(FGameplayTag InGameplayTag)
{
	if (auto* FoundData = EventAnimMontageData.Find(InGameplayTag))
	{
		return *FoundData;
	}
	
	return FEventAnimMontageData();
}
