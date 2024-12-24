// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Jump.h"

#include "GameFramework/Character.h"
#include "GAS/AbilityTask/KYAT_JumpAndWaitForLanding.h"

UKYGA_Jump::UKYGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UKYGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(!bResult)
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UKYGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKYAT_JumpAndWaitForLanding* JumpAndWaitForLandingTask = UKYAT_JumpAndWaitForLanding::CreateTask(this);

	JumpAndWaitForLandingTask->OnComplete.AddDynamic(this, &UKYGA_Jump::OnSimpleCompleteCallback);
	JumpAndWaitForLandingTask->ReadyForActivation();
}

void UKYGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}
