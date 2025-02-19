// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KYCharacterBase.h"
#include "Interface/KYTargetableInterface.h"
#include "KYCharacterNonPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExecutedAbilityEndDelegate);

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYCharacterNonPlayer : public AKYCharacterBase, public IKYTargetableInterface
{
	GENERATED_BODY()

public:
	AKYCharacterNonPlayer(const FObjectInitializer& ObjectInitializer);

	void OnExecuteTagChanged(FGameplayTag GameplayTag, int Count);
	
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION(BlueprintCallable)
	bool ExecuteGameplayAbilityFromClass(TSubclassOf<UGameplayAbility> InAbilityClass);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FExecutedAbilityEndDelegate OnExecutedAbilityEnd;

	UFUNCTION(BlueprintCallable)
	void PlayExecutedMontage(FName SectionName);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExecutableState(bool IsExecutable);

	UFUNCTION()
	void OnExecutedMontageEndCallback(UAnimMontage* Montage, bool bInterrupted);
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDead() override;
	
	virtual void OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void UpdateTargetedStatus(bool InStatus) override;

	virtual void DropBountyItem();
	
protected:
	UPROPERTY()
	TObjectPtr<class UKYAttributeSetEnemy> AttributeSetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DropBountyEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UKYWidgetComponent> HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UKYWidgetComponent> TargetedWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USceneComponent> ExecuteMotionWarpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ExecutedMontage;
};
