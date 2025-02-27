// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/KYItemObjectBase.h"
#include "KYItemDrop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYItemDrop : public AKYItemObjectBase
{
	GENERATED_BODY()

public:
	AKYItemDrop();

protected:
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector SpawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ApplyDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsHoldPosition;

private:
	bool bIsApplied;
};
