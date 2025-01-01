// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_ComboAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_ComboAttack::UKYGA_ComboAttack()
{
	MaxCombo = 0;
	OriginTags = AbilityTags;
}

void UKYGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AbilityTags = OriginTags;
	
	CurrentActorInfo = ActorInfo;
	CurrentCombo = 0;
	HasNextComboInput = false;

	AbilityTags.AddTag(ComboAttackTypes[CurrentCombo]);
}

void UKYGA_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (SourceASC->GetOwnedGameplayTags().HasTagExact(KYTAG_EVENT_COMBO_AVAILABLE) && !HasNextComboInput)
	{
		HasNextComboInput = true;
		UAbilityTask_WaitGameplayTagRemoved* ComboValidTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, KYTAG_EVENT_COMBO_AVAILABLE);
		ComboValidTask->Removed.AddDynamic(this, &UKYGA_ComboAttack::InputCallback);
		ComboValidTask->ReadyForActivation();
	}
}


void UKYGA_ComboAttack::InputCallback_Implementation()
{
	StartNextCombo();
}

FName UKYGA_ComboAttack::GetNextSection()
{
	AbilityTags.RemoveTag(ComboAttackTypes[CurrentCombo]);
	
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 0, MaxCombo);
	FName NextSection = *FString::Printf(TEXT("Attack%d"), CurrentCombo);

	AbilityTags.AddTag(ComboAttackTypes[CurrentCombo]);
	return NextSection;
}

void UKYGA_ComboAttack::StartNextCombo()
{
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		HasNextComboInput = false;
	}
}

