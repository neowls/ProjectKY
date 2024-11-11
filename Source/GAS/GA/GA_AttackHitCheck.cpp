// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_AttackHitCheck.h"
#include "GAS.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GA/AT/AT_Trace.h"
#include "Attribute/GASCharacterAttributeSet.h"
#include "TA/TA_Trace.h"
#include "Tag/GASGameplayTag.h"

UGA_AttackHitCheck::UGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;
	
	UAT_Trace* AttackTraceTask = UAT_Trace::CreateTask(this, TargetActorClass);

	AttackTraceTask->OnTraceComplete.AddDynamic(this, &UGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
	
}

void UGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	if(UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		GAS_LOG(LogGAS, Log, TEXT("Target %s Detected"), *HitResult.GetActor()->GetName());


		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		
		if(!SourceASC || !TargetASC)
		{
			GAS_LOG(LogGAS, Error, TEXT("ASC Not Found!"));
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
			return;
		}

		const UGASCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UGASCharacterAttributeSet>();
		/*
		UGASCharacterAttributeSet* TargetAttribute = const_cast<UGASCharacterAttributeSet*>(TargetASC->GetSet<UGASCharacterAttributeSet>());

		if(!SourceAttribute || !TargetAttribute)
		{
			GAS_LOG(LogGAS, Error, TEXT("Attribute Not Found!"));
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRange();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
		*/

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);	//Damage Effect 전달
		if(EffectSpecHandle.IsValid())
		{
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(GASTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate()); // SetByCaller 인 경우 GameplayTag 를 통해 적용된다.
			// ReSharper disable once CppExpressionWithoutSideEffects
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			TargetASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);	// Attack Buff Effect 전달
		if(BuffEffectSpecHandle.IsValid())
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}
	else if(UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);	//Damage Effect 전달
		if(EffectSpecHandle.IsValid())
		{
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(GASTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate()); // SetByCaller 인 경우 GameplayTag 를 통해 적용된다.
			// ReSharper disable once CppExpressionWithoutSideEffects
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
