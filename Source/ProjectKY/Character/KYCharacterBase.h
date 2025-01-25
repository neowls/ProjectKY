// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Struct/KYStruct.h"
#include "KYCharacterBase.generated.h"

class UKYCharacterBaseAsset;
class UKYAT_DamageReaction;
class UKYCharacterMovementComponent;

UCLASS()
class PROJECTKY_API AKYCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYCharacterBase(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UKYCharacterMovementComponent* GetKYCharacterMovement() const { return KYCharacterMovement; }

	FORCEINLINE UStaticMeshComponent* GetWeaponComponent() const { return WeaponComp; }

	FORCEINLINE void ClearIgnoreActors() { HitIgnoreActors.Empty(); }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	/*
	UFUNCTION()
	virtual void OnStanceEvent(AActor* Causer, const FGameplayTagContainer& GameplayTagContainer, uint8 CurrentStanceState);
	*/
	
	UFUNCTION()
	virtual void OutOfHealth();
	
	UFUNCTION(BlueprintCallable, Category="Animation")
	FORCEINLINE FEventAnimMontageData GetAnimMontageData(FGameplayTag InGameplayTag);

protected:
	virtual void SetDead();

	virtual void GiveStartAbilities();	// 기본 어빌리티 부여

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHitFlash();

	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category=GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
	
	UPROPERTY(EditAnywhere, Category=Animation)
	UKYCharacterBaseAsset* AnimMontageAsset;
	
	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	TObjectPtr<UKYCharacterMovementComponent> KYCharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Weapon)
	UStaticMeshComponent* WeaponComp;

	UPROPERTY(BlueprintReadWrite, Category=Collision, meta=(AllowPrivateAccess="true"))
	TSet<AActor*> HitIgnoreActors;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameplayEffect> InitStatEffect;

};
