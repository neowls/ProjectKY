// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/KYPlayerController.h"
#include "ProjectKY.h"
#include "GameFramework/PlayerState.h"
#include "UI/KYHUDUserWidget.h"

AKYPlayerController::AKYPlayerController()
{
	SetShowMouseCursor(true);
	InitializeClassFinder(HUDWidgetClass, TEXT("/Game/_Dev/UI/WBP_HUD.WBP_HUD_C"));

}

void AKYPlayerController::SetupHUDWidget()
{
	HUDWidget = CreateWidget<UKYHUDUserWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	HUDWidget->SetAbilitySystemComponent(PlayerState);
}

void AKYPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetupHUDWidget();
	
}
