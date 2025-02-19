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

void UKYGA_Attack::OnSimpleEventReceivedCallback_Implementation(FGameplayEventData Payload)
{
	Super::OnSimpleEventReceivedCallback_Implementation(Payload);

	if(IsValid(AttackGameplayEffect[CurrentAttackIndex]))
	{
		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload.TargetData, AttackGameplayEffect[CurrentAttackIndex], CurrentAttackIndex + 1.0f);
		
		FTimerHandle SlowTimer;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
		GetWorld()->GetTimerManager().SetTimer(SlowTimer, [this]() { UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1); }, 0.005f, false);
			
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Can't find Gameplay Effect, Please Add It."))
	}
}