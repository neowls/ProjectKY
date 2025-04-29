// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"

class UAbilityTask_WaitGameplayEvent;

UKYGA_Attack::UKYGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentAttackIndex = 0;
	bIsCombatAbility = true;
	bInputAbility = true;
}

bool UKYGA_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (Cast<AKYCharacterBase>(ActorInfo->AvatarActor)->GetCurrentWeaponType() == EKYWeaponSubType::None) Result = false;
	return Result;
}


void UKYGA_Attack::OnSimpleEventReceivedCallback_Implementation(FGameplayEventData Payload)
{
	Super::OnSimpleEventReceivedCallback_Implementation(Payload);

	if(IsValid(AttackGameplayEffect))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, AttackGameplayEffect, CurrentAttackIndex + 1.0f);
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find Gameplay Effect, Please Add It."))
	}
}
