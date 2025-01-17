// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KYAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYAIController : public AAIController
{
	GENERATED_BODY()
public:
	AKYAIController();
	
	void RunAI();
	void StopAI();
	
	void SetHitStatus(bool bIsHit);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	
private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
};
