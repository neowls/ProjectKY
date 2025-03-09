// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Character/KYCharacterBase.h"
#include "GAS/Tag/KYGameplayTag.h"


UKYGameplayAbility::UKYGameplayAbility()
{
	OnAbilityLevelUp.AddDynamic(this, &ThisClass::OnAbilityLevelUpCallback);
}

void UKYGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if(bPassiveAbility) TryActivatePassiveAbility(ActorInfo, Spec);
}

void UKYGameplayAbility::TryActivatePassiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	const bool bIsPredicting = (Spec.ActivationInfo.ActivationMode == EGameplayAbilityActivationMode::Predicting);
 
	// Try to activate if activation type is Passive. Passive abilities are auto activated when given.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting)
	{
		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();
 
		// If avatar actor is torn off or about to die, don't try to activate it.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);
 
			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;
 
			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UKYGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AKYCharacterBase* Character = Cast<AKYCharacterBase>(ActorInfo->AvatarActor);
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC) return;
	
	if (bIsCombatAbility)
	{
		if (ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsCombat)) GetWorld()->GetTimerManager().ClearTimer(Character->GetCurrentCombatTimerHandle());
		else
		{
			ASC->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsCombat);
		}
	}
}

void UKYGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (bIsCombatAbility) UpdateCombatStateTag(ActorInfo);
}

void UKYGameplayAbility::OnAbilityLevelUpCallback()
{
	// 레벨 업 콜백 처리
}

void UKYGameplayAbility::UpdateCombatStateTag(const FGameplayAbilityActorInfo* ActorInfo)
{
	AKYCharacterBase* Character = Cast<AKYCharacterBase>(ActorInfo->AvatarActor);
	UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();
	if (!ASC) return;
	
	if (ASC->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsCombat))
	{
		GetWorld()->GetTimerManager().SetTimer(Character->GetCurrentCombatTimerHandle(), [ASC]()
		{
			if (ASC)
			{
				ASC->RemoveLooseGameplayTag(UKYGameplayTags::CharacterState.IsCombat);
			}
		}, 5.0f, false);
	}
}


void UKYGameplayAbility::OnSimpleInterruptedCallback_Implementation()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UKYGameplayAbility::OnSimpleCompleteCallback_Implementation()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}