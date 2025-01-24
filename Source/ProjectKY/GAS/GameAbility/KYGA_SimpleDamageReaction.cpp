// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_SimpleDamageReaction.h"
#include "ProjectKY.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/KYCharacterBase.h"

UKYGA_SimpleDamageReaction::UKYGA_SimpleDamageReaction()
{
	
}


void UKYGA_SimpleDamageReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	KY_LOG(LogKY, Log, TEXT("Instigator Name : %s"), *TriggerEventData->Instigator.GetName());

	AKYCharacterBase* CharacterBase = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo());
	if(CharacterBase)
	{
		FGameplayTag HitTag;
		for (const auto& iter : AbilityTags)
		{
			HitTag = iter;
		}
		
		FVector TargetLocation = TriggerEventData->Instigator->GetActorLocation() - CharacterBase->GetActorLocation();
		
		CharacterBase->SetActorRotation(TargetLocation.Rotation());

		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SimpleHitReaction"), CharacterBase->GetAnimMontageByTag(HitTag));
		PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteCallback);
		PlayMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnSimpleInterruptedCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnSimpleInterruptedCallback);
		PlayMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnSimpleInterruptedCallback);

		PlayMontageTask->ReadyForActivation();
	}
}
