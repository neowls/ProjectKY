// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/KYGA_MeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS.h"
#include "Character/GASComboActionData.h"
#include "Character/GASCharacterPlayer.h"
//#include "GameFramework/CharacterMovementComponent.h"

UKYGA_MeleeAttack::UKYGA_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UKYGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AGASCharacterPlayer* GASCharacter = CastChecked<AGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	CurrentActorInfo = ActorInfo;
	CurrentComboData = GASCharacter->GetComboActionData();
	
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), GASCharacter->GetAttackMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UKYGA_MeleeAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UKYGA_MeleeAttack::OnInterruptedCallback);
	
	
	PlayAttackTask->ReadyForActivation();
	
	StartComboTimer();
}

void UKYGA_MeleeAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(ComboEffectiveTimerHandle.IsValid())
	{
		if(ComboAvailableTimerHandle.IsValid())
		{
			HasNextComboInput = true;			
		}
		else if(IsComboAvailable)
		{
			StartNextCombo();
		}
	}
	else
	{
		HasNextComboInput = false;
	}
}

void UKYGA_MeleeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UKYGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
	GAS_LOG(LogGAS, Log, TEXT("End Ability"));
} 


FName UKYGA_MeleeAttack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UKYGA_MeleeAttack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	const float ComboAvailableTime = CurrentComboData->AvailableFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	
	if(ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboEffectiveTimerHandle, this, &UKYGA_MeleeAttack::CloseComboEffectiveTime, ComboEffectiveTime, false);
	}

	if(ComboAvailableTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboAvailableTimerHandle, this, &UKYGA_MeleeAttack::CheckComboInput, ComboAvailableTime, false);
	}
}

void UKYGA_MeleeAttack::CheckComboInput()
{
	ComboAvailableTimerHandle.Invalidate();
	if(HasNextComboInput)
	{
		StartNextCombo();
	}
	else
	{
		IsComboAvailable = true;
	}
}

void UKYGA_MeleeAttack::StartNextCombo()
{
	MontageJumpToSection(GetNextSection());
	StartComboTimer();
	HasNextComboInput = false;
	IsComboAvailable = false;
}

void UKYGA_MeleeAttack::CloseComboEffectiveTime()
{
	ComboEffectiveTimerHandle.Invalidate();
	HasNextComboInput = false;
}


