// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/KYPrimaryDataAsset.h"
#include "KYAbilityDataAsset.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYAbilityDataAsset : public UKYPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TMap<FName, TSubclassOf<UGameplayAbility>> AbilityNameMap;
};
