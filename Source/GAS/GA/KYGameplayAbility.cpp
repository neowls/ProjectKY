// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/KYGameplayAbility.h"

UKYGameplayAbility::UKYGameplayAbility()
{
	
}

void UKYGameplayAbility::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}

void UKYGameplayAbility::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
