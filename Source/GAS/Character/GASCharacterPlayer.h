// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "InputActionValue.h"
#include "GASCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASCharacterPlayer : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	AGASCharacterPlayer();
	FORCEINLINE virtual class UAnimMontage* GetSkillMontage() const { return SkillMontage; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetupGASInputComponent();

	FORCEINLINE UAnimMontage* GetAttackMontage() const { return MainAttackMontage; }
	FORCEINLINE class UGASComboActionData* GetComboActionData() const { return ComboActionData; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


#pragma region BASE
protected:
	virtual void Move(const FInputActionValue& Value);
	virtual void Look();
	
#pragma endregion 
#pragma region INPUT
protected:
	
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

protected:
	UPROPERTY(EditAnywhere, Category=GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;
	
	UPROPERTY(VisibleAnywhere, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MainAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SubAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillAction;

#pragma endregion

#pragma region MONTAGE
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Montage)
	TObjectPtr<UAnimMontage> MainAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGASComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Montage)
	TObjectPtr<class UAnimMontage> SkillMontage;

#pragma endregion

	UPROPERTY(EditAnywhere, Category=GAS)
	TSubclassOf<class UGameplayAbility> SkillAbilityClass;
};
