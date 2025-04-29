#pragma once
#include "CoreMinimal.h"
#include "KYConfirmDialogEntryObject.generated.h"


UCLASS()
class PROJECTKY_API UKyConfirmDialogEntryObject : public UObject
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	FName ActionID;

	UPROPERTY(Transient)
	FText DisplayText;
};
