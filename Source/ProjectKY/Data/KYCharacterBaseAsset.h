// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Struct/KYStruct.h"
#include "KYCharacterBaseAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYCharacterBaseAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<FGameplayTag, FEventAnimMontageData> EventAnimMontageData;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	FEventAnimMontageData GetEventAnimMontageData(FGameplayTag InGameplayTag);
	
};
