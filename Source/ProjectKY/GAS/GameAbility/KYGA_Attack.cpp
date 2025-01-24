// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
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
	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, TEXT("PlayAttackMontage"), AttackMontage, KYTAG_EVENT_ATTACKHIT, AttackSpeed);
	
	
	PMT->EventReceived.AddDynamic(this, &UKYGA_Attack::AttackHitEventCallback);
	PMT->OnCompleted.AddDynamic(this, &UKYGA_Attack::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &UKYGA_Attack::OnSimpleCompleteEventCallback);

	/*UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttackMontage"), AttackMontage, AttackSpeed);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UKYGA_Attack::OnSimpleCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UKYGA_Attack::OnSimpleInterruptedCallback);
	
	UAbilityTask_WaitGameplayEvent* HitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KYTAG_EVENT_ATTACKHIT, nullptr, false, true);
	HitCheckTask->EventReceived.AddDynamic(this, &UKYGA_Attack::AttackHitEventCallback);
	
	PlayAttackTask->ReadyForActivation();
	HitCheckTask->ReadyForActivation();*/

	PMT->ReadyForActivation();
}


void UKYGA_Attack::AttackHitEventCallback(FGameplayEventData Payload)
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

