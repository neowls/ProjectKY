// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KYCharacterBase.h"
#include "InputActionValue.h"
#include "KYCharacterPlayer.generated.h"

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

protected:
	virtual void GiveStartAbilities() override;

	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	virtual void SetDead() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void CurrentWeaponTrailState(const FGameplayTag CallbackTag, int32 NewCount);

	void CurrentWeaponTrailState_Implementation(const FGameplayTag CallbackTag, int32 NewCount);
	
protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
	
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> GuardAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
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
	TObjectPtr<UInputAction> SkillAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, Category=Camera)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, Category=Debug)
	bool bShowGASDebug = true;

	UPROPERTY(BlueprintReadOnly)
	bool bCurrentTrailState = false;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset = FRotator(0.0f, -45.0f, 0.0f);
};
