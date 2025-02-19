// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_AnimBase.h"

#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_AnimBase::UKYGA_AnimBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UKYGA_AnimBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	PlayAnimMontageTask();
}

void UKYGA_AnimBase::PlayAnimMontageTask()
{
	AbilityTags.Filter(FGameplayTagContainer(KYTAG_DATA));
	const FGameplayTag AbilityTag = AbilityTags.First();
	
	if (AbilityTag == FGameplayTag())
	{
		KY_LOG(LogKY, Warning, TEXT("%s Ability Does Not Have Data Tag."), *this->GetName());
		return;
	}

	FEventAnimMontageData PlayEventMontageData = Cast<AKYCharacterBase>(CurrentActorInfo->AvatarActor)->GetAnimMontageData(AbilityTag);
	
	if (PlayEventMontageData.Montage == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("%s Tag Has No Anim Montage."), *AbilityTag.GetTagName().ToString());
	}
	
	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, PlayEventMontageData);

	PMT->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	PMT->OnBlendOut.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	if (PlayEventMontageData.IsUseEvent) PMT->EventReceived.AddDynamic(this, &ThisClass::OnSimpleEventReceivedCallback);
	PMT->ReadyForActivation();
}


void UKYGA_AnimBase::OnSimpleEventReceivedCallback_Implementation(FGameplayEventData Payload)
{
	
}

void UKYGA_AnimBase::OnSimpleCompleteEventCallback(FGameplayEventData Payload)
{
	OnSimpleCompleteCallback();
}

void UKYGA_AnimBase::OnSimpleInterruptEventCallback(FGameplayEventData Payload)
{
	OnSimpleInterruptedCallback();
}
