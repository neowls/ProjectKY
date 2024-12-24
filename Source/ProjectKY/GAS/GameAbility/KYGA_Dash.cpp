// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Dash.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/KYCharacterMovementComponent.h"
#include "Character/KYCharacterPlayer.h"

UKYGA_Dash::UKYGA_Dash()
{
	
}

void UKYGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AKYCharacterPlayer* Character = CastChecked<AKYCharacterPlayer>(ActorInfo->AvatarActor.Get());
	UAnimMontage* MontageToPlay = Character->GetKYCharacterMovement()->MovementMode == MOVE_Walking ? GroundDashMontage :AirDashMontage;

	UAbilityTask_PlayMontageAndWait* PlayDashTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayDashMontage"), MontageToPlay, 1.0f);
	PlayDashTask->OnCompleted.AddDynamic(this, &UKYGA_Dash::OnSimpleCompleteCallback);
	PlayDashTask->OnInterrupted.AddDynamic(this, &UKYGA_Dash::OnSimpleInterruptedCallback);

	PlayDashTask->ReadyForActivation();
}
