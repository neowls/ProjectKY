#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "KYStruct.generated.h"


USTRUCT(BlueprintType)
struct FEventAnimMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	FName TaskInstanceName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	TObjectPtr<class UAnimMontage> Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	float PlayRate = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	FName StartSectionName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info")
	bool IsUseEvent = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim Montage Info", meta=(EditCondition="IsUseEvent", EditConditionHides))
	FGameplayTag EventTag = FGameplayTag();
};
