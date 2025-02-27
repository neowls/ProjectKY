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
	bIsCombatAbility = false;
	bInputAbility = true;
}

bool UKYGA_AnimBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	AbilityTags.Filter(FGameplayTagContainer(UKYGameplayTags::Data.Data));
	const FGameplayTag AbilityTag = AbilityTags.First();
	
	if (AbilityTag == FGameplayTag())
	{
		KY_LOG(LogKY, Warning, TEXT("%s Ability Does Not Have Data Tag."), *this->GetName());
		Result = false;
	}
	
	AnimMontageData = Cast<AKYCharacterBase>(CurrentActorInfo->AvatarActor)->GetAnimMontageData(AbilityTag);
	
	if (AnimMontageData.Montage == nullptr)
	{
		KY_LOG(LogKY, Warning, TEXT("%s Tag Has No Anim Montage."), *AbilityTag.GetTagName().ToString());
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

	PlayAnimMontageTask();
	if (bIsCombatAbility) ApplyCombatEffect();
}

void UKYGA_AnimBase::PlayAnimMontageTask()
{
	AbilityTags.Filter(FGameplayTagContainer(UKYGameplayTags::Data.Data));
	const FGameplayTag AbilityTag = AbilityTags.First();
	
	if (AbilityTag == FGameplayTag())
	{
		KY_LOG(LogKY, Warning, TEXT("%s Ability Does Not Have Data Tag."), *this->GetName());
		return;
	}
	
	UKYAT_PlayMontageAndWaitForEvent* PMT = UKYAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, AnimMontageData);

	PMT->OnCompleted.AddDynamic(this, &ThisClass::OnSimpleCompleteEventCallback);
	PMT->OnInterrupted.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	PMT->OnBlendOut.AddDynamic(this, &ThisClass::OnSimpleInterruptEventCallback);
	if (AnimMontageData.IsUseEvent) PMT->EventReceived.AddDynamic(this, &ThisClass::OnSimpleEventReceivedCallback);
	PMT->ReadyForActivation();
}

void UKYGA_AnimBase::ApplyCombatEffect()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CombatEffect, 1.0f, EffectContextHandle); // 이펙트 부여
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
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
