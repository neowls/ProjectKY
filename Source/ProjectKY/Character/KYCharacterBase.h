// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Data/KYCharacterWeaponAnimAsset.h"
#include "GameplayTagContainer.h"
#include "Struct/KYStruct.h"
#include "KYCharacterBase.generated.h"

class UKYCharacterWeaponAnimAsset;
class UKYAT_DamageReaction;
class UKYCharacterMovementComponent;

UENUM(BlueprintType)
enum class EWeaponCollisionState : uint8
{
	LeftOn,
	LeftOff,
	RightOn,
	RightOff
};

UCLASS()
class PROJECTKY_API AKYCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKYCharacterBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UKYCharacterMovementComponent* GetKYCharacterMovement() const { return KYCharacterMovement; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE UStaticMeshComponent* GetWeaponComponent(bool bIsRight) const { return bIsRight ? WeaponCompRight : WeaponCompLeft; }
	
	UFUNCTION(BlueprintCallable, Category="Weapon", meta=(BlueprintThreadSafe))
	FORCEINLINE EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }
	
	UFUNCTION(BlueprintCallable, Category="Animation")
	FORCEINLINE FEventAnimMontageData GetAnimMontageData(FGameplayTag InGameplayTag);

	UFUNCTION(BlueprintCallable, Category="Collision")
	FORCEINLINE void ClearIgnoreActors() { HitIgnoreActors.Empty(); }
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);
	
	UFUNCTION()
	virtual void OutOfHealth();

	UFUNCTION(BlueprintCallable, Category=GAS)
	virtual void GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass);

	UFUNCTION(BlueprintGetter, Category=GAS, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsCombat() const { return bIsCombat;}

	UFUNCTION()
	void SetWeaponCollisionState(EWeaponCollisionState InState);

protected:
	virtual void BeginPlay() override;
	
	virtual void SetDead();

	virtual void GrantStartAbilities();	// 기본 어빌리티 부여
	
	virtual void InitializeStatEffect();	// 기본 스탯 부여

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHitFlash();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateWeaponData(FWeaponData NewWeapon);

	UFUNCTION()
	void UpdateWeaponSocket();
	
	void OnCombatState(const FGameplayTag GameplayTag, int32 Count);
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, Category=GAS)
	TArray<TSubclassOf<UKYGameplayAbility>> StartAbilities;

	UPROPERTY(EditDefaultsOnly, Category=GAS)
	TArray<TSubclassOf<class UGameplayEffect>> InitStatEffect;

	UPROPERTY(EditDefaultsOnly, Category=GAS)
	TMap<EWeaponType, TObjectPtr<UKYCharacterWeaponAnimAsset>> OwnWeaponAnimAssets;
	
	UPROPERTY(VisibleAnywhere, Category=Animation)
	EWeaponType CurrentWeaponType = EWeaponType::None;
	
	UPROPERTY(EditAnywhere, Category=Animation)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	TObjectPtr<UKYCharacterMovementComponent> KYCharacterMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	UStaticMeshComponent* WeaponCompRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	UStaticMeshComponent* WeaponCompLeft;

	UPROPERTY(BlueprintReadWrite, Category=Collision, meta=(AllowPrivateAccess="true"))
	TSet<AActor*> HitIgnoreActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GAS)
	bool bIsCombat;

};