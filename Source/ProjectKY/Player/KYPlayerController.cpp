
#include "Player/KYPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ProjectKY.h"
#include "GameFramework/PlayerState.h"
#include "UI/KYHUDUserWidget.h"
#include "UI/Window/KYStatusWindowWidget.h"




AKYPlayerController::AKYPlayerController()
{
	InitializeClassFinder(HUDWidgetClass, TEXT("/Game/_Dev/UI/WBP/WBP_HUD.WBP_HUD_C"));
	InitializeObjectFinder(WindowAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_Window.IA_Window'"));
	InitializeObjectFinder(WindowSelectAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowSelect.IA_WindowSelect'"));
	InitializeObjectFinder(WindowBackAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowBack.IA_WindowBack'"));
	InitializeObjectFinder(WindowNavigateAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowNavigate.IA_WindowNavigate'"));
	InitializeObjectFinder(WindowTabSwitchAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowTabSwitch.IA_WindowTabSwitch'"));
	InitializeObjectFinder(WindowContext, TEXT("/Game/_Dev/Input/IMC_PlayerWindow.IMC_PlayerWindow"));
	InitializeObjectFinder(GameplayContext, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/_Dev/Input/IMC_Player.IMC_Player'"));
	
	SetShowMouseCursor(true);
}


void AKYPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetupWidget();
}

void AKYPlayerController::SetupWidget()
{
	// HUD 위젯 생성 및 부착
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UKYHUDUserWidget>(this, HUDWidgetClass);
       	HUDWidget->SetAbilitySystemComponent(PlayerState);
       	HUDWidget->AddToViewport();
	}

	// Status 위젯 생성 및 부착
	if (StatusWindowWidgetClass)
	{
		StatusWindowWidget = CreateWidget<UKYStatusWindowWidget>(this, StatusWindowWidgetClass);
		StatusWindowWidget->SetAbilitySystemComponent(PlayerState);
		StatusWindowWidget->AddToViewport();
		StatusWindowWidget->DeactivateWindow();
	}
}

void AKYPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetWindowInputContext(GameplayContext);
    
	// 입력 액션 바인딩
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 상태 창 토글
		EnhancedInput->BindAction(WindowAction, ETriggerEvent::Started, this, &ThisClass::ToggleStatusWindow);
        
		// 상태 창 탭 전환 (Q, R 키)
		EnhancedInput->BindAction(WindowTabSwitchAction, ETriggerEvent::Triggered, this, &ThisClass::SwitchStatusWindowTab);

		// 상태 창 내부 이동 (WASD 키)
		EnhancedInput->BindAction(WindowNavigateAction, ETriggerEvent::Triggered, this, &ThisClass::NavigateStatusWindowTab);
        
		// 상태 창 선택
		EnhancedInput->BindAction(WindowSelectAction, ETriggerEvent::Started, this, &ThisClass::SelectStatusWindow);
        
		// 상태 창 뒤로가기
		EnhancedInput->BindAction(WindowBackAction, ETriggerEvent::Started, this, &ThisClass::BackStatusWindow);
	}
}

void AKYPlayerController::ToggleStatusWindow()
{
	if (!StatusWindowWidget) 
	{
		KY_LOG(LogKY, Warning, TEXT("StatusWindowWidget이 null입니다."));
		return;
	}
	
	if(IsPaused())
	{
		KY_LOG(LogKY, Log, TEXT("상태 창 닫기 시도"));
		SetPause(false);
		StatusWindowWidget->DeactivateWindow();
		SetWindowInputContext(GameplayContext);
		KY_LOG(LogKY, Log, TEXT("상태 창 닫기 완료"));
	}
	else
	{
		KY_LOG(LogKY, Log, TEXT("상태 창 열기 시도"));
		SetPause(true);
		StatusWindowWidget->ActivateWindow();
		SetWindowInputContext(WindowContext);
		KY_LOG(LogKY, Log, TEXT("상태 창 열기 완료"));
	}
}

void AKYPlayerController::SwitchStatusWindowTab(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;

	//	키 입력 처리
	float MoveValue = InputActionValue.Get<float>();
	
	if (MoveValue > 0.0f)
	{
		//	오른쪽 이동 (R 키)
		StatusWindowWidget->SwitchToNextTab();
		KY_LOG(LogKY, Log, TEXT("Right"));
	}
	else if (MoveValue < 0.0f)
	{
		//	왼쪽 이동 (Q 키)
		StatusWindowWidget->SwitchToPreviousTab();
		KY_LOG(LogKY, Log, TEXT("Left"));
	} 
}

void AKYPlayerController::NavigateStatusWindowTab(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;

	FVector2d NavigateValue = InputActionValue.Get<FVector2D>();
	if(IKYInputHandlerInterface::Execute_HandleNavigationInput(StatusWindowWidget, NavigateValue.X, NavigateValue.Y))
	{
		KY_LOG(LogKY, Log, TEXT("Navigate %s"), *NavigateValue.ToString());
		return;
	}
}

void AKYPlayerController::SelectStatusWindow(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;

	// 현재 활성화된 탭의 아이템 선택 처리
	if(IKYInputHandlerInterface::Execute_HandleConfirmInput(StatusWindowWidget))
	{
		KY_LOG(LogKY, Log, TEXT("Select"));
		return;
	}
}

void AKYPlayerController::BackStatusWindow(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;

	// 현재 활성화된 탭 내부의 탭에서 되돌아가기 선택 처리 (아이템 사용, 장비 적용 등)
	if(IKYInputHandlerInterface::Execute_HandleConfirmInput(StatusWindowWidget))
	{
		KY_LOG(LogKY, Log, TEXT("Cancel"));
		return;
	}
	
	// 그저 활성화된 탭이라면 상태 창 닫기
	if (IsPaused())
	{
		SetPause(false);
		StatusWindowWidget->DeactivateWindow();
		SetWindowInputContext(GameplayContext);
	}
}


void AKYPlayerController::SetWindowInputContext(const TObjectPtr<UInputMappingContext>& Context)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(Context, 1);
		
		KY_LOG(LogKY, Log, TEXT("Context : %s"), *Context.GetName());
	}
}
