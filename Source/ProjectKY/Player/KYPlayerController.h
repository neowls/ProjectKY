// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KYPlayerController.generated.h"

class UKYHUDUserWidget;

/**
 * 
 */
UCLASS()
class PROJECTKY_API AKYPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKYPlayerController();

	FORCEINLINE class UKYHUDUserWidget* GetHUDWidget() const {  return HUDWidget; }

	virtual void SetupHUDWidget();

protected:
	virtual void OnPossess(APawn* InPawn) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UKYHUDUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TObjectPtr<UKYHUDUserWidget> HUDWidget;
};
