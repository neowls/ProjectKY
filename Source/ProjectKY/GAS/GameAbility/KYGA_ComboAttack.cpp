// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_ComboAttack.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"

UKYGA_ComboAttack::UKYGA_ComboAttack()
{
	MaxCombo = 0;
}

void UKYGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentAttackIndex = 0;
	MaxCombo = GetCurrentMontage()->GetNumSections();
	HasNextComboInput = false;
}

void UKYGA_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (SourceASC->GetOwnedGameplayTags().HasTagExact(UKYGameplayTags::Event.Combo_Available) && !HasNextComboInput)
	{
		HasNextComboInput = true;
		UAbilityTask_WaitGameplayTagRemoved* ComboValidTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, UKYGameplayTags::Event.Combo_Available);
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
	CurrentAttackIndex = FMath::Clamp(CurrentAttackIndex + 1, 0, MaxCombo);
	const FName NextSection = *FString::Printf(TEXT("Attack%d"), CurrentAttackIndex);
	
	return NextSection;
}

void UKYGA_ComboAttack::StartNextCombo()
{
	if (HasNextComboInput)
	{
		MontageJumpToSection(GetNextSection());
		if (bIsCombatAbility) ApplyCombatEffect();
		HasNextComboInput = false;
	}
}

