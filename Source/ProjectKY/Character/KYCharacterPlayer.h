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

	FORCEINLINE UStaticMeshComponent* GetWeaponComponent() const { return WeaponComp; }
	FORCEINLINE void ClearIgnoreActors() { HitIgnoreActors.Empty(); }

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void SetupGASInputComponent();

protected:
	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

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
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DashAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SubAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UpperAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SkillAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Weapon)
	UStaticMeshComponent* WeaponComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(EditAnywhere, Category=Camera)
	TObjectPtr<class USpringArmComponent> SpringArmComp;

	UPROPERTY(BlueprintReadWrite, Category=Collision, meta=(AllowPrivateAccess="true"))
	TSet<AActor*> HitIgnoreActors;
};

