// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameAbility/KYGameplayAbility.h"
#include "KYGA_Aiming.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API UKYGA_Aiming : public UKYGameplayAbility
{
	GENERATED_BODY()

public:
	UKYGA_Aiming();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void OnTargetCompleteCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category="GAS")
	TSubclassOf<class AKYTA_Trace> TargetActorClass;
	
	UPROPERTY(EditAnywhere, Category="GAS")
	float TargetingRange;

	UPROPERTY(EditAnywhere, Category="GAS")
	float TargetingRadius;


	UPROPERTY(EditAnywhere, Category="GAS")
	float TargetingAngleThreshold;

	UFUNCTION()
	void AimAtCursor(const FGameplayAbilityActorInfo& ActorInfo);
	
	UFUNCTION()
	void RotateCharacter(FVector& TargetLocation);

	UFUNCTION()
	void SetTargetUIStatus(bool InStatus);

	UFUNCTION()
	void CheckCurrentTarget();
	
private:
	bool bIsTargeting;
	
	FVector CursorLocation;

	UPROPERTY()
	FGameplayAbilityTargetDataHandle CurrentTargetDataHandle;

	UPROPERTY()
	TObjectPtr<class AKYTA_Trace> TargetedActor;

	
};

