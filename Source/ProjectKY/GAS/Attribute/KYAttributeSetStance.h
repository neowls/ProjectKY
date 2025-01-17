#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "KYAttributeSetStance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStanceChangeEvent, AActor*, StanceCauser, const FGameplayTagContainer&, GameplayTagContainer, uint8, StanceState);

/**
 * 
 */

UCLASS()
class PROJECTKY_API UKYAttributeSetStance : public UKYAttributeSetBase
{
	GENERATED_BODY()

public:
	UKYAttributeSetStance();

	ATTRIBUTE_ACCESSORS(UKYAttributeSetStance, Stance);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetStance, MaxStance);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetStance, StaggerStance);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetStance, KnockDownStance);
	ATTRIBUTE_ACCESSORS(UKYAttributeSetStance, IncomingStance);

	mutable FStanceChangeEvent OnStanceChange;


protected:
	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Stance;

	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxStance;

	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData StaggerStance;

	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData KnockDownStance;

	UPROPERTY(BlueprintReadWrite, Category="Attributes", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData IncomingStance;
	

protected:
	virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	uint8 GetCurrentStanceState() const;
};
