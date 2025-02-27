// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KYAbilityDataAsset.h"

FPrimaryAssetId UKYAbilityDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("KYAbilityDataList", GetFName());
}
