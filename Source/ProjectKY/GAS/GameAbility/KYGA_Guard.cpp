// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Guard.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_Guard::UKYGA_Guard()
{
	
}

void UKYGA_Guard::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;

	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, TEXT("PlayAttackMontage"), GuardMontage, KYTAG_EVENT_HIT);

	PMT->EventReceived.AddDynamic(this, &UKYGA_Guard::OnGuardEventCallback);
	PMT->OnCompleted.AddDynamic(this, &UKYGA_Guard::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &UKYGA_Guard::OnSimpleCompleteEventCallback);
}

void UKYGA_Guard::OnGuardEventCallback(FGameplayEventData Payload)
{
	
}
