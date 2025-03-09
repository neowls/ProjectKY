// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_AnimBase.h"

#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_AnimBase::UKYGA_AnimBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsCombatAbility = true;
	bInputAbility = true;
}



void UKYGA_AnimBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	AbilityTags.Filter(FGameplayTagContainer(UKYGameplayTags::Data.Data));
	DataTag = AbilityTags.First();

	AKYCharacterBase* Character = Cast<AKYCharacterBase>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		Character->OnWeaponAnimSetChanged.AddDynamic(this, &UKYGA_AnimBase::OnAnimSetChangeCallback);
	}
}

void UKYGA_AnimBase::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);

	AKYCharacterBase* Character = Cast<AKYCharacterBase>(ActorInfo->AvatarActor.Get());
	if (Character)
	{
		Character->OnWeaponAnimSetChanged.RemoveDynamic(this, &UKYGA_AnimBase::OnAnimSetChangeCallback);
	}
}


bool UKYGA_AnimBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	
	if (DataTag == FGameplayTag())
	{
		KY_LOG(LogKY, Warning, TEXT("%s Ability Does Not Have Data Tag."), *this->GetName());
		Result = false;
	}

	if(AnimMontageData.Montage == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("%s Ability Does Not Have Montage Data"), *this->GetName());
		Result = false;
	}

	return Result;
}

void UKYGA_AnimBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// 코스트, 쿨다운, 이펙트로 인한 실행 불가
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	UAnimInstance* AnimInstance = GetActorInfo().AnimInstance.Get();
	if (AnimInstance && AnimInstance->Montage_IsPlaying(AnimMontageData.Montage))
	{
		AnimInstance->Montage_SetPosition(AnimMontageData.Montage, 0.0f);
	}
	else
	{
		PlayAnimMontageTask();
	}
}

void UKYGA_AnimBase::OnAnimSetChangeCallback()
{
	if (GetCurrentActorInfo())
	{
		AnimMontageData = Cast<AKYCharacterBase>(GetAvatarActorFromActorInfo())->GetAnimMontageData(DataTag);
	}
}

void UKYGA_AnimBase::PlayAnimMontageTask()
{
	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, AnimMontageData);

	PMT->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	PMT->OnBlendOut.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	if (AnimMontageData.IsUseEvent) PMT->EventReceived.AddDynamic(this, &ThisClass::OnSimpleEventReceivedCallback);
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
