// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KYPrimaryDataAsset.h"

FPrimaryAssetId UKYPrimaryDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("UKYPrimaryDataAsset", GetFName());
}
