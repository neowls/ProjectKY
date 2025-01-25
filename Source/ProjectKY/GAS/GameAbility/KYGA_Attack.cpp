// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"

class UAbilityTask_WaitGameplayEvent;

UKYGA_Attack::UKYGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentAttackIndex = 0;
}


void UKYGA_Attack::OnSimpleEventReceivedCallback(FGameplayEventData Payload)
{
	if(IsValid(AttackGameplayEffect[CurrentAttackIndex]))
	{
		TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, AttackGameplayEffect[CurrentAttackIndex], 1.0f);
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find Gameplay Effect, Please Add It."))
	}
}