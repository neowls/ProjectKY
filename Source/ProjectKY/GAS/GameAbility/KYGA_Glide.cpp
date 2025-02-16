// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Glide.h"

#include "ProjectKY.h"
#include "Character/KYCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UKYGA_Glide::UKYGA_Glide()
{
	
}

bool UKYGA_Glide::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	AKYCharacterPlayer* Character = Cast<AKYCharacterPlayer>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Result = Character->GetCharacterMovement()->Velocity.Z < 0.0f;
	}
	return Result;
}

void UKYGA_Glide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	KY_LOG(LogKY, Log, TEXT("Activate"));
	AKYCharacterPlayer* Character = Cast<AKYCharacterPlayer>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->GlideShowWingStatus(true);
		Character->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	}
}

void UKYGA_Glide::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	OnSimpleCompleteCallback();
}

void UKYGA_Glide::OnLandedCallback(const FHitResult& Hit)
{
	OnSimpleCompleteCallback();
}

void UKYGA_Glide::OnSimpleCompleteCallback_Implementation()
{
	AKYCharacterPlayer* Character = Cast<AKYCharacterPlayer>(GetAvatarActorFromActorInfo());
	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);
	if (Character)
	{
		Character->GlideShowWingStatus(false);
	}
	Super::OnSimpleCompleteCallback_Implementation();
}


