// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameAbility/KYGameplayAbility.h"

UKYGameplayAbility::UKYGameplayAbility()
{
	
}

void UKYGameplayAbility::OnSimpleCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UKYGameplayAbility::OnSimpleInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
