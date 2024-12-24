// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_UpperAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UKYGA_UpperAttack::UKYGA_UpperAttack()
{
	LaunchScale = 100.0f;
}

void UKYGA_UpperAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayUpperTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayUpperAttackMontage"), UpperAttackMontage);
	PlayUpperTask->OnCompleted.AddDynamic(this, &UKYGA_UpperAttack::OnSimpleCompleteCallback);
	PlayUpperTask->OnInterrupted.AddDynamic(this, &UKYGA_UpperAttack::OnSimpleInterruptedCallback);

	//PlayUpperTask->ReadyForActivation();
}

void UKYGA_UpperAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UKYGA_UpperAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
