// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Guard.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/AbilityTask/KYAT_PlayMontageAndWaitForEvent.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_Guard::UKYGA_Guard()
{
	
}

void UKYGA_Guard::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGUARDEND)) return;
	
	MontageJumpToSection(FName("GuardEnd"));
}

void UKYGA_Guard::OnSimpleEventReceivedCallback(const FGameplayEventData Payload)
{
	Super::OnSimpleEventReceivedCallback(Payload);
	
	if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISPARRY)) // 먼저 패링중인지 확인한다.
	{
		MontageJumpToSection(FName("Parry"));
		CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Character.Hit.Parry"));
		
		KY_LOG(LogKY, Log, TEXT("Parry Success"));
	}
	else if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(KYTAG_CHARACTER_ISGUARD)) // 가드 중일 경우 타입에 따라 반응을 나눈다.
	{
		if (Payload.EventTag == KYTAG_CHARACTER_ATTACK_HEAVY)
		{
			MontageJumpToSection(FName("GuardBreak"));
		}
		else
		{
			MontageJumpToSection(FName("GuardHit"));
			CurrentActorInfo->AbilitySystemComponent->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Character.Hit.Guard"));
		}
	}
}

