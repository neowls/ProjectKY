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
	
	UFUNCTION(BlueprintCallable)
	void InitAI();

	UFUNCTION(BlueprintCallable)
	void RunAI();

	UFUNCTION(BlueprintCallable)
	void StopAI();
	
	void SetHitStatus(bool bIsHit);

	UFUNCTION(BlueprintPure)
	FORCEINLINE AActor* GetTargetActor() const { return Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_TARGET)); }

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	bool bIsAutoTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent;
	
	
private:
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
};
