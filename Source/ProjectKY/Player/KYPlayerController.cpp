
#include "Player/KYPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ProjectKY.h"
#include "GameFramework/PlayerState.h"
#include "UI/KYHUDUserWidget.h"
#include "UI/KYStatusWindowWidget.h"




AKYPlayerController::AKYPlayerController()
{
	SetShowMouseCursor(true);
	InitializeClassFinder(HUDWidgetClass, TEXT("/Game/_Dev/UI/WBP/WBP_HUD.WBP_HUD_C"));
	InitializeObjectFinder(WindowAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_Window.IA_Window'"));
	InitializeObjectFinder(WindowSelectAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowSelect.IA_WindowSelect'"));
	InitializeObjectFinder(WindowBackAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowBack.IA_WindowBack'"));
	InitializeObjectFinder(WindowMoveAction, TEXT("/Script/EnhancedInput.InputAction'/Game/_Dev/Input/IA_WindowMove.IA_WindowMove'"));
	
	InitializeObjectFinder(WindowContext, TEXT("/Game/_Dev/Input/IMC_PlayerWindow.IMC_PlayerWindow"));
	InitializeObjectFinder(GameplayContext, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/_Dev/Input/IMC_Player.IMC_Player'"));
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

	// Status 위젯 생성
	if (StatusWindowWidgetClass)
	{
		StatusWindowWidget = CreateWidget<UKYStatusWindowWidget>(this, StatusWindowWidgetClass);
	}
}

void AKYPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// 입력 매핑 컨텍스트 추가
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
        
		// 기본 게임플레이 컨텍스트 추가
		if(GameplayContext)
		{
			Subsystem->AddMappingContext(GameplayContext, 1);
		}
	}
    
    
	// 입력 액션 바인딩
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 상태 창 토글
		EnhancedInput->BindAction(WindowAction, ETriggerEvent::Started, this, &ThisClass::ToggleStatusWindow);
        
		// 상태 창 탭 이동 (Q, R 키 또는 WASD)
		EnhancedInput->BindAction(WindowMoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveStatusWindowTab);
        
		// 상태 창 선택
		EnhancedInput->BindAction(WindowSelectAction, ETriggerEvent::Started, this, &ThisClass::SelectStatusWindow);
        
		// 상태 창 뒤로가기
		EnhancedInput->BindAction(WindowBackAction, ETriggerEvent::Started, this, &ThisClass::BackStatusWindow);
	}
}

void AKYPlayerController::ToggleStatusWindow()
{
	if (!StatusWindowWidget) return;
    
	if (StatusWindowWidget->IsInViewport())
	{
		// 상태 창 닫기
		StatusWindowWidget->DeactivateWidget();
		StatusWindowWidget->RemoveFromParent();
		SetWindowInputContext(GameplayContext);
	}
	else
	{
		// 상태 창 열기
		StatusWindowWidget->AddToViewport();
		StatusWindowWidget->ActivateWidget();
		SetWindowInputContext(WindowContext);
	}
}

void AKYPlayerController::MoveStatusWindowTab(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;
	
	if (InputActionValue.GetValueType() == EInputActionValueType::Axis2D)
	{
		// WASD 키 입력 처리
		FVector2D MoveValue = InputActionValue.Get<FVector2D>();
        
		// 좌우 이동만 처리
		if (MoveValue.X > 0.0f)
		{
			// 오른쪽 이동 (R 키와 동일)
			StatusWindowWidget->SwitchToNextTab();
		}
		else if (MoveValue.X < 0.0f)
		{
			// 왼쪽 이동 (Q 키와 동일)
			StatusWindowWidget->SwitchToPreviousTab();
		}
	}
}

void AKYPlayerController::SelectStatusWindow(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;
    
	// 현재 활성화된 탭의 아이템 선택 처리
	// 필요에 따라 StatusWindowWidget에 선택 처리 함수 추가
}

void AKYPlayerController::BackStatusWindow(const FInputActionValue& InputActionValue)
{
	if (!StatusWindowWidget || !StatusWindowWidget->IsInViewport()) return;

	// 현재 활성화된 탭 내부의 탭에서 되돌아가기 선택 처리 (아이템 사용, 장비 적용 등)

	/*// 그저 활성화된 탭이라면 상태 창 닫기
	StatusWindowWidget->DeactivateWidget();
	StatusWindowWidget->RemoveFromParent();
	SetWindowInputContext(GameplayContext);*/
}


void AKYPlayerController::SetWindowInputContext(const TObjectPtr<UInputMappingContext>& Context)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(Context, 0);
	}
}
