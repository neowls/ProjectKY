// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KYPrimaryDataAsset.h"
#include "Struct/KYStruct.h"
#include "KYCharacterWeaponAnimAsset.generated.h"

class UAnimInstance;

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYCharacterWeaponAnimAsset : public UKYPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UKYCharacterWeaponAnimAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TMap<FGameplayTag, FEventAnimMontageData> EventAnimMontageData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> WeaponAnimClass;
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	FEventAnimMontageData GetEventAnimMontageData(FGameplayTag InGameplayTag);
	
};
