// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GASComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGASComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UGASComboActionData();

	UPROPERTY(EditAnywhere, Category=Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category=Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category=Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category=Name)
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category=Name)
	TArray<float> AvailableFrameCount;
};
