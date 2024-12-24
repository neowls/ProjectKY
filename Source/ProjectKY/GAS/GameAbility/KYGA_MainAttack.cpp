// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_MainAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/KYCharacterPlayer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Character/KYCharacterMovementComponent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_MainAttack::UKYGA_MainAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UKYGA_MainAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AKYCharacterPlayer* Character = CastChecked<AKYCharacterPlayer>(ActorInfo->AvatarActor.Get());
	CurrentActorInfo = ActorInfo;
	CurrentCombo = 0;
	HasNextComboInput = false;
	
	UAnimMontage* MontageToPlay = Character->GetKYCharacterMovement()->MovementMode == MOVE_Walking ? GroundAttackMontage : AirAttackMontage;

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttackMontage"), MontageToPlay, 1.2f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UKYGA_MainAttack::OnSimpleCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UKYGA_MainAttack::OnSimpleInterruptedCallback);

	UAbilityTask_WaitGameplayEvent* HitCheckTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, KYTAG_EVENT_ATTACKHIT, nullptr, false, true);
	HitCheckTask->EventReceived.AddDynamic(this, &UKYGA_MainAttack::AttackHitCheckCallback);

	PlayAttackTask->ReadyForActivation();
	HitCheckTask->ReadyForActivation();
	
}

void UKYGA_MainAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (SourceASC->GetOwnedGameplayTags().HasTagExact(KYTAG_EVENT_COMBO_AVAILABLE) && !HasNextComboInput)
	{
		HasNextComboInput = true;
		UAbilityTask_WaitGameplayTagRemoved* ComboValidTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, KYTAG_EVENT_COMBO_AVAILABLE);
		ComboValidTask->Removed.AddDynamic(this, &UKYGA_MainAttack::StartNextCombo);
		ComboValidTask->ReadyForActivation();
	}
	
}

void UKYGA_MainAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UKYGA_MainAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UKYGA_MainAttack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 0, 2);
	FName NextSection = *FString::Printf(TEXT("Attack%d"), CurrentCombo);
	return NextSection;
}

void UKYGA_MainAttack::StartNextCombo()
{
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		HasNextComboInput = false;
	}
}

void UKYGA_MainAttack::AttackHitCheckCallback(FGameplayEventData Payload)
{
	ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, TargetHitEffect, 1.0f);
	KY_LOG(LogKY, Log, TEXT("Hit Check Callback"));
}
