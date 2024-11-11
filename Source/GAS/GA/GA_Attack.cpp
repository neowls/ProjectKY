// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Attack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS.h"
#include "Character/GASComboActionData.h"
#include "Character/GASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Attack::UGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AGASCharacterPlayer* GASCharacter = CastChecked<AGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	CurrentActorInfo = ActorInfo;
	CurrentComboData = GASCharacter->GetComboActionData();
	
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), GASCharacter->GetAttackMontage(), 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UGA_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UGA_Attack::OnInterruptedCallback);
	
	PlayAttackTask->ReadyForActivation();
	
	StartComboTimer();
}

void UGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
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

void UGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
	
} 

void UGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

FName UGA_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	return NextSection;
}

void UGA_Attack::StartComboTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	const float ComboAvailableTime = CurrentComboData->AvailableFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	
	if(ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboEffectiveTimerHandle, this, &UGA_Attack::CloseComboEffectiveTime, ComboEffectiveTime, false);
	}

	if(ComboAvailableTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboAvailableTimerHandle, this, &UGA_Attack::CheckComboInput, ComboAvailableTime, false);
	}
}

void UGA_Attack::CheckComboInput()
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

void UGA_Attack::StartNextCombo()
{
	MontageJumpToSection(GetNextSection());
	StartComboTimer();
	HasNextComboInput = false;
	IsComboAvailable = false;
}

void UGA_Attack::CloseComboEffectiveTime()
{
	ComboEffectiveTimerHandle.Invalidate();
	HasNextComboInput = false;
}


