// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Attack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

class UAbilityTask_WaitGameplayEvent;

UKYGA_Attack::UKYGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AttackSpeed = 1.2f;
}


void UKYGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttackMontage"), AttackMontage, AttackSpeed);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UKYGA_Attack::OnSimpleCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UKYGA_Attack::OnSimpleInterruptedCallback);
	
	UAbilityTask_WaitGameplayEvent* HitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KYTAG_EVENT_ATTACKHIT, nullptr, false, true);
	HitCheckTask->EventReceived.AddDynamic(this, &UKYGA_Attack::AttackHitCheckCallback);

	PlayAttackTask->ReadyForActivation();
	HitCheckTask->ReadyForActivation();
}


void UKYGA_Attack::AttackHitCheckCallback(FGameplayEventData Payload)
{
	ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, TargetHitEffect, 1.0f);
}
