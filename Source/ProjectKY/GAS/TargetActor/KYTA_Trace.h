// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "KYTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AKYTA_Trace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
	virtual void ConfirmTargetingAndContinue() override;

	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }

	void SetUseShape(bool InShape) { bUseShape = InShape; }

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	
	bool bShowDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseShape = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseMulti = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
	float TraceRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace", meta=(EditCondition="bShapeTrace", EditConditionHides))
	float TraceRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
	float CenterOffset = 0.0f;
	
};
