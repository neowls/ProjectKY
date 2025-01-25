// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UKYGA_SimpleDamageReaction::UKYGA_SimpleDamageReaction()
{
	
}


void UKYGA_SimpleDamageReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	KY_LOG(LogKY, Log, TEXT("Instigator Name : %s"), *TriggerEventData->Instigator.GetName());
	FVector TargetLocation = TriggerEventData->Instigator->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation();
	GetAvatarActorFromActorInfo()->SetActorRotation(TargetLocation.Rotation());
}
