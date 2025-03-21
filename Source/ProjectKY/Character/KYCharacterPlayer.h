// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Character/KYCharacterBase.h"
#include "InputActionValue.h"
#include "MotionWarping/Public/MotionWarpingComponent.h"
#include "KYCharacterPlayer.generated.h"

class UKYGameplayAbility;
class UInputAction;

UCLASS()
class PROJECTKY_API AKYCharacterPlayer : public AKYCharacterBase
{
	GENERATED_BODY()

public:
	AKYCharacterPlayer(const FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void SetupGASInputComponent();

	virtual void GrantAbility(TSubclassOf<UKYGameplayAbility> NewAbilityClass, float Level = 1.0f, bool bAddToTagMap = false) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GlideShowWingStatus(bool InStatus);

	UFUNCTION(Category="Input", meta=(BlueprintThreadSafe))
	FORCEINLINE FVector GetInputDirection() const;

	UFUNCTION(Category="Camera", meta=(BlueprintThreadSafe))
	FORCEINLINE FRotator GetRotationOffset() const { return RotationOffset; } 

	virtual void AddWeaponData(const FName& WeaponName, const FWeaponData& InWeaponData) override; // 무기 데이터 추가

	virtual void OnCombatState(const FGameplayTag GameplayTag, int32 Count) override; // 전투 상태 태그 업데이트 이벤트

protected: 
	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	void ChangeWeaponWithType(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractObject();

	UFUNCTION()
	void OnLevelUpApplyStat();
	
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	void GASInputPressed(FGameplayTag InputTag);
	void GASInputReleased(FGameplayTag InputTag);

	virtual void SetDead_Implementation() override;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=GAS)
	TObjectPtr<class UCurveTable> PlayerLevelCurveTable;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	TArray<FName> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	uint8 CurrentWeaponIndex;

#pragma region INPUT
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GuardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GlideAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HeavyAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LightAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UpperAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RangeAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeWeaponAction;
	

#pragma endregion 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trigger)
	TObjectPtr<class USphereComponent> ItemTriggerComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Trigger)
	TObjectPtr<class UBoxComponent> InteractTriggerComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	TObjectPtr<class UStaticMeshComponent> WingComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Animation)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;
	
	UPROPERTY(EditAnywhere, Category=Debug)
	bool bShowGASDebug = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator RotationOffset = FRotator(0.0f, -45.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=GAS)
	TSubclassOf<UGameplayEffect> LevelUpEffect;

private:
	UPROPERTY()
	TSet<int32> OwnAbilityId;
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedInteractableTargets;
    
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentInteractableTarget;
	

	UFUNCTION()
	void OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
	UFUNCTION()
	void OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void UpdateClosestInteractableTarget(); // 상호작용 가능한 가장 가까운 액터 지정   

	void ExecutionTargetedEnemy(); // 처형 어빌리티 실행 및 모션 워핑 업데이트
};


