// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Jump.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_Jump::UKYGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bInputAbility = true;
}

bool UKYGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(!bResult)
	{
		return false;
	}
	
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	
	return Character->CanJump();
}

void UKYGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		OnSimpleInterruptedCallback();
		return;
	}
	
	Character->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	Character->OnReachedJumpApex.AddDynamic(this, &ThisClass::OnApexCallback);

	for (auto* iter : Character->LandedDelegate.GetAllObjects())
	{
		KY_LOG(LogKY, Log, TEXT("Landed Delegate Object : %s"), *iter->GetName());
	}
	
	Character->GetCharacterMovement()->bNotifyApex = true;

	Character->Jump();
	
	if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsJumping))
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsJumping);
	}
}

void UKYGA_Jump::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character && !Character->CanJump()) return;
	
	Character->GetCharacterMovement()->bNotifyApex = true;
	Character->Jump();
	
	if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsJumping))
	{
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(UKYGameplayTags::CharacterState.IsJumping);
	}
}

void UKYGA_Jump::OnLandedCallback(const FHitResult& Hit)
{
	KY_LOG(LogKY, Log, TEXT("Land"));
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		OnSimpleInterruptedCallback();
		return;
	}

	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
	Character->OnReachedJumpApex.RemoveDynamic(this, &ThisClass::OnApexCallback);
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsJumping))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(UKYGameplayTags::CharacterState.IsJumping);
	}
	OnSimpleCompleteCallback();
}

void UKYGA_Jump::OnApexCallback()
{
	KY_LOG(LogKY, Log, TEXT("Apex"));
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		OnSimpleInterruptedCallback();
		return;
	}
	
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(UKYGameplayTags::CharacterState.IsJumping))
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(UKYGameplayTags::CharacterState.IsJumping);
	}
}

void UKYGA_Jump::OnAbilityLevelUpCallback()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	Character->JumpMaxCount = GetAbilityLevel();
}

