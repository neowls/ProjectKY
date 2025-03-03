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

	void OnExecutableTagChanged(FGameplayTag GameplayTag, int Count);
	
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION(BlueprintCallable)
	bool ExecuteGameplayAbilityFromClass(TSubclassOf<UGameplayAbility> InAbilityClass);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FExecutedAbilityEndDelegate OnExecutedAbilityEnd;

	UFUNCTION(BlueprintCallable)
	void PlayExecutedMontage(FName SectionName, UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintImplementableEvent)
	void OnExecutableState(bool IsExecutable);

	UFUNCTION()
	void OnExecutedMontageEndCallback(UAnimMontage* Montage, bool bInterrupted);

	virtual void GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level = 1.0f, bool bAddToTagMap = false) override;
	
protected:
	virtual void SetDead_Implementation() override;
	
	virtual void OnHitTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void UpdateTargetedStatus(bool InStatus) override;

	virtual void DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage) override;

	virtual void DropBountyItem();
	
	virtual void RegisterGameplayEvents() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UKYAttributeSetEnemy> AttributeSetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DropBountyEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UKYWidgetComponent> HPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UKYWidgetComponent> TargetedWidget;
};
