// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_AnimBase.h"

#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_AnimBase::UKYGA_AnimBase()
{
}


void UKYGA_AnimBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AbilityTags.Filter(FGameplayTagContainer(KYTAG_DATA));
	FGameplayTag AbilityTag;
	for (auto& Tag : AbilityTags)
	{
		AbilityTag = Tag;
	}
	
	if (AbilityTag == FGameplayTag())
	{
		KY_LOG(LogKY, Warning, TEXT("This Ability Does Not Have Data Tag."));
		return;
	}
	
	FEventAnimMontageData AnimMontageData = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo())->GetAnimMontageData(AbilityTag);
	
	if (AnimMontageData.Montage == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("%s Tag Has No Anim Montage."), *AbilityTag.GetTagName().ToString());
		return;
	}

	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, AnimMontageData);

	PMT->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	if (AnimMontageData.IsUseEvent) PMT->EventReceived.AddDynamic(this, &ThisClass::OnSimpleEventReceivedCallback);
	PMT->OnBlendOut.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	PMT->ReadyForActivation();
}


void UKYGA_AnimBase::OnSimpleCompleteEventCallback(FGameplayEventData Payload)
{
	OnSimpleCompleteCallback();
}

void UKYGA_AnimBase::OnSimpleInterruptEventCallback(FGameplayEventData Payload)
{
	OnSimpleInterruptedCallback();
}
