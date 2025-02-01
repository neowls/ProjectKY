// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/KYInteractableInterface.h"
#include "Item/KYItemObjectBase.h"
#include "KYItemInteract.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYItemInteract : public AKYItemObjectBase, public IKYInteractableInterface
{
	GENERATED_BODY()

public:
	AKYItemInteract();

protected:
	virtual void InteractEvent_Implementation(AActor* Causer) override;

	virtual void SetInteractableStatus_Implementation(bool InStatus) override;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interactable)
	TObjectPtr<class UKYWidgetComponent> InteractWidget;

	
};
