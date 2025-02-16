// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KYCharacterBaseAsset.h"

#include "ProjectKY.h"

FEventAnimMontageData UKYCharacterBaseAsset::GetEventAnimMontageData(FGameplayTag InGameplayTag)
{
	if (FEventAnimMontageData* FoundData = EventAnimMontageData.Find(InGameplayTag))
	{
		return *FoundData;
	}

	KY_LOG(LogKY, Warning, TEXT("Can't Find Event Montage Data"));
	return FEventAnimMontageData();
	
}
