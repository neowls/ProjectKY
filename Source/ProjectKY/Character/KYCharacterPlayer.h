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
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GlideShowWingStatus(bool InStatus);
	
protected:
	virtual void GiveStartAbilities() override;

	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void InteractObject();
	
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	virtual void SetDead() override;

protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

#pragma region INPUT
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
	

#pragma endregion 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, Category=Camera)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere, Category=Trigger)
	TObjectPtr<class USphereComponent> ItemTriggerComp;

	UPROPERTY(EditAnywhere, Category=Trigger)
	TObjectPtr<class UBoxComponent> InteractTriggerComp;
	
	UPROPERTY(EditAnywhere, Category=Debug)
	bool bShowGASDebug = true;
	
	UPROPERTY(EditAnywhere)
	FRotator RotationOffset = FRotator(0.0f, -45.0f, 0.0f);

};
