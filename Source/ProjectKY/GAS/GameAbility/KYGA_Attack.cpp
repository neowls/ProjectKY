// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGA_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "GAS/Tag/KYGameplayTag.h"
#include "Kismet/GameplayStatics.h"

class UAbilityTask_WaitGameplayEvent;

UKYGA_Attack::UKYGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentAttackIndex = 0;
}


void UKYGA_Attack::OnSimpleEventReceivedCallback(FGameplayEventData Payload)
{
	KY_LOG(LogKY, Log, TEXT("Tag : %s"), *Payload.EventTag.GetTagName().ToString());
	
	if(IsValid(AttackGameplayEffect[CurrentAttackIndex]))
	{
		TArray<FActiveGameplayEffectHandle> EffectHandles = ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, AttackGameplayEffect[CurrentAttackIndex], CurrentAttackIndex + 1.0f);
		FTimerHandle SlowTimer;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		GetWorld()->GetTimerManager().SetTimer(SlowTimer, [this]() { UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1); }, 0.01f, false);
			
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find Gameplay Effect, Please Add It."))
	}
}