// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_SimpleDamageReaction::UKYGA_SimpleDamageReaction()
{
	
}


void UKYGA_SimpleDamageReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const FVector TargetLocation = TriggerEventData->Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();
	GetAvatarActorFromActorInfo()->SetActorRotation(TargetLocation.Rotation());
	KY_LOG(LogKY,Log, TEXT("Tag : %s"), *TriggerEventData->EventTag.GetTagName().ToString());

	if(!TriggerEventData->EventTag.MatchesTag(KYTAG_CHARACTER_ATTACK_PARRY)) CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Character.Hit.Light"));
}

void UKYGA_SimpleDamageReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISSTAGGERED))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(KYTAG_CHARACTER_ISSTAGGERED, 99);
	}
}
