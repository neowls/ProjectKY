// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KYInteractableInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractEvent);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKYInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTKY_API IKYInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	FOnInteractEvent OnInteractEvent;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetInteractableStatus(bool InStatus);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractEvent(AActor* Causer);
	
};
