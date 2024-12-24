// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "KYCharacterBase.generated.h"

class UKYCharacterMovementComponent;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

UCLASS()
class PROJECTKY_API AKYCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYCharacterBase(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UKYCharacterMovementComponent* GetKYCharacterMovement() const { return KYCharacterMovement; }

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);
	
	UFUNCTION()
	virtual void OutOfHealth();

protected:
	virtual void SetDead();
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category=GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	TObjectPtr<UKYCharacterMovementComponent> KYCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation, meta=(AllowPrivateAccess=true))
	TMap<EHitDirection,TObjectPtr<UAnimMontage>> HitMontage;
	
};
