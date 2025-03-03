// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_SimpleDamageReaction::UKYGA_SimpleDamageReaction()
{
	bIsCombatAbility = true;
	bInputAbility = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bRetriggerInstancedAbility = true;
}

void UKYGA_SimpleDamageReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const FVector TargetLocation = TriggerEventData->Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();
	FRotator TargetRotation = TargetLocation.Rotation();
	TargetRotation.Pitch = 0.0f;
	GetAvatarActorFromActorInfo()->SetActorRotation(TargetRotation);

	if(!TriggerEventData->EventTag.MatchesTag(UKYGameplayTags::CharacterState.IsParry)) CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Character.Hit.Light"));
}

void UKYGA_SimpleDamageReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsStaggered))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(UKYGameplayTags::CharacterState.IsStaggered, 99);
	}
}