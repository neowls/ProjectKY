// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Data/KYCharacterWeaponAnimAsset.h"
#include "Struct/KYStruct.h"
#include "KYCharacterBase.generated.h"

class UKYGameplayAbility;
class UKYCharacterWeaponAnimAsset;
class UKYAT_DamageReaction;
class UKYCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponAnimSetChanged);

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

	UPROPERTY(BlueprintAssignable)
	FOnWeaponAnimSetChanged OnWeaponAnimSetChanged;

	UFUNCTION(BlueprintCallable)
	virtual void AddWeaponData(const FName& WeaponName, const FWeaponData& InWeaponData) { OwnWeaponData.Add(WeaponName, InWeaponData); } 

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UKYCharacterMovementComponent* GetKYCharacterMovement() const { return KYCharacterMovement; }
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE UStaticMeshComponent* GetWeaponComponent(bool bIsRight) const { return bIsRight ? WeaponCompRight : WeaponCompLeft; }
	
	UFUNCTION(BlueprintCallable, Category="Weapon", meta=(BlueprintThreadSafe))
	FORCEINLINE EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }
	
	UFUNCTION(BlueprintCallable, Category="Animation")
	FORCEINLINE FEventAnimMontageData GetAnimMontageData(FGameplayTag InGameplayTag);

	FORCEINLINE bool CheckAnimMontageData(const FGameplayTag& InGameplayTag) { return OwnWeaponAnimAssets.FindRef(CurrentWeaponType)->CheckEventAnimMontageData(InGameplayTag); }

	UFUNCTION(BlueprintCallable, Category="Collision")
	FORCEINLINE void ClearIgnoreActors() { HitIgnoreActors.Empty(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FTimerHandle& GetCurrentCombatTimerHandle() { return CombatTagTimerHandle; }
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);
	
	UFUNCTION(BlueprintCallable)
	virtual void OutOfHealth();
	
	UFUNCTION(BlueprintCallable, Category = GAS)
	virtual void GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level = 1.0f, bool bAddToTagMap = false);

	UFUNCTION(BlueprintGetter, Category=GAS, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsCombat() const { return bIsCombat;}

	UFUNCTION(BlueprintCallable, Category=GAS)
	void UpdateWeaponSocket(bool IsRight = true, FName TargetSocket = TEXT("Weapon_R"));

	UFUNCTION()
	void SetWeaponCollisionState(EWeaponCollisionState InState);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDead();
	
	virtual void GrantStartAbilities();	// 기본 어빌리티 부여
	
	virtual void InitializeStatEffect();	// 기본 스탯 부여

	virtual void InitializeAbilitySystemComponent();

	virtual void RegisterGameplayEvents();

	void ChangeWeapon(FName WeaponName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHitFlash();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateWeaponData(FWeaponData NewWeapon);
	
	virtual void OnCombatState(const FGameplayTag GameplayTag, int32 Count);

	UFUNCTION()
	virtual void OnWeaponCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void UpgradeAbility(FGameplayTag& InGameplayTag);
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, Category=GAS)
	TArray<TSubclassOf<UKYGameplayAbility>> StartAbilities;

	UPROPERTY(EditDefaultsOnly, Category=GAS)
	TArray<TSubclassOf<class UGameplayEffect>> InitStatEffect;

	UPROPERTY()
	TMap<FGameplayTag, FGameplayAbilitySpecHandle> AbilitiesTagMap;

	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	TMap<EWeaponType, TObjectPtr<UKYCharacterWeaponAnimAsset>> OwnWeaponAnimAssets;
	
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	EWeaponType CurrentWeaponType = EWeaponType::None;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TMap<FName, FWeaponData> OwnWeaponData;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Material)
	UMaterialInstanceDynamic* MeshDynamicMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Material)
	UMaterialInstanceDynamic* WeaponRightDynamicMaterialInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Material)
	UMaterialInstanceDynamic* WeaponLeftDynamicMaterialInstance;

	UPROPERTY()
	FTimerHandle CombatTagTimerHandle;

private:
	UFUNCTION()
	void SetDynamicMaterialInstance();

	UPROPERTY(Transient)
	FGameplayEventData AttackEventData;

	UPROPERTY(Transient)
	FGameplayEventData DamageEventData;
};