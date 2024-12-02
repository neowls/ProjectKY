// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/KYGA_Skill.h"
#include "Character/GASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UKYGA_Skill::UKYGA_Skill()
{
}

void UKYGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AGASCharacterPlayer* TargetCharacter = Cast<AGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	ActiveSkillActionMontage = TargetCharacter->GetSkillMontage();
	if(!ActiveSkillActionMontage)
	{
		return;
	}
	//TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UKYGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UKYGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();
}

void UKYGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AGASCharacterPlayer* TargetCharacter = Cast<AGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if(TargetCharacter)
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);		
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
