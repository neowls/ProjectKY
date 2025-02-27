// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Jump.h"

#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"
#include "GameFramework/Character.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"

UKYGA_Jump::UKYGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bInputAbility = true;
}

void UKYGA_Jump::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	Cast<ACharacter>(ActorInfo->AvatarActor)->JumpMaxCount = JumpMaxCount;
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
	return Character->CanJump();
}

void UKYGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->Jump();
	Character->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	KY_LOG(LogKY, Log, TEXT("Activate"));
}

void UKYGA_Jump::OnLandedCallback(const FHitResult& Hit)
{
	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
