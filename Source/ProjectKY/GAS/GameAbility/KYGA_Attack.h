// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KYGA_AnimBase.h"
#include "KYGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Attack : public UKYGA_AnimBase
{
	GENERATED_BODY()

public:
	UKYGA_Attack();
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual void OnSimpleEventReceivedCallback_Implementation(FGameplayEventData Payload) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> AttackGameplayEffect;
	
	uint8 CurrentAttackIndex;
};