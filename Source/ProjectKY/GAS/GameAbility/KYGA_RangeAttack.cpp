// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_RangeAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/KYCharacterPlayer.h"

UKYGA_RangeAttack::UKYGA_RangeAttack()
{
	
}

void UKYGA_RangeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AKYCharacterPlayer* Character = CastChecked<AKYCharacterPlayer>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayRangeAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayRangeAttackMontage"), GroundSlashMontage);
	PlayRangeAttackTask->OnCompleted.AddDynamic(this, &UKYGA_RangeAttack::OnSimpleCompleteCallback);
	PlayRangeAttackTask->OnInterrupted.AddDynamic(this, &UKYGA_RangeAttack::OnSimpleInterruptedCallback);

	PlayRangeAttackTask->ReadyForActivation();
}

void UKYGA_RangeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
}

void UKYGA_RangeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
