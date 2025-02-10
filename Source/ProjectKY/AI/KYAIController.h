// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KYAI.h"
#include "BehaviorTree/BlackboardComponent.h"
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

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetTargetActor() const { return Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_TARGET)); }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	bool bIsAutoTarget;
	
private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
};
