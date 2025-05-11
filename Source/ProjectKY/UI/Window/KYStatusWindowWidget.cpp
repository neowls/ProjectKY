
#include "UI/Window/KYStatusWindowWidget.h"
#include "UI/Window/Tab/KYAbilitiesTabWidget.h"
#include "UI/Window/Tab/KYStatsTabWidget.h"
#include "UI/KYButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "ProjectKY.h"
#include "Components/HorizontalBox.h"


UKYStatusWindowWidget::UKYStatusWindowWidget()
{
	ActiveTabIndex = 0;
	ActiveTabWidget = nullptr;
	
}

void UKYStatusWindowWidget::ActivateWindow()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

void UKYStatusWindowWidget::DeactivateWindow()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
}


void UKYStatusWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	//	탭 버튼 초기화
	InitializeTabButtons();

	//	탭 위젯 초기화
	InitializeTabWidgets();
	
	// 초기 가시성 및 히트 테스트 설정
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	// 초기 탭 선택
	HandleTabSwitchInput_Implementation(ActiveTabIndex);
}

void UKYStatusWindowWidget::InitializeTabWidgets()
{
	// 탭 위젯 초기화
	TabWidgets.Empty();
	if (!TabSwitcher)
	{
		KY_LOG(LogKY, Warning, TEXT("Please Binding `TabSwitcher`."));
		return;
	}

	int32 TabIndex = 0;

	// 스탯 탭 위젯 생성
	if(StatsTabWidgetClass)
	{
		StatsTabWidget = CreateWidget<UKYStatsTabWidget>(this, StatsTabWidgetClass);
		if (StatsTabWidget)
		{
			TabSwitcher->AddChild(StatsTabWidget);
			if(StatsTabWidget->Implements<UKYInputHandlerInterface>())
			{
				TabWidgets.Add(TabIndex++, StatsTabWidget);
			}
			StatsTabWidget->SetAbilitySystemComponent(*ASC);
			StatsTabWidget->SetupAttributeWidgets();
		}
	}

	// 어빌리티 탭 위젯 생성
	if (AbilitiesTabWidgetClass)
	{
		AbilitiesTabWidget = CreateWidget<UKYAbilitiesTabWidget>(this, AbilitiesTabWidgetClass);
		if (AbilitiesTabWidget)
		{
			TabSwitcher->AddChild(AbilitiesTabWidget);
			if (AbilitiesTabWidget->Implements<UKYInputHandlerInterface>())
			{
				TabWidgets.Add(TabIndex++, AbilitiesTabWidget);
			}
			AbilitiesTabWidget->SetAbilitySystemComponent(*ASC);
		}
	}

	
	// 인벤토리 탭 위젯 생성
	if (InventoryTabWidgetClass)
	{
		InventoryTabWidget = CreateWidget<UKYUserWidget>(this, InventoryTabWidgetClass);
		if (InventoryTabWidget)
		{
			TabSwitcher->AddChild(InventoryTabWidget);
			if (InventoryTabWidget->Implements<UKYInputHandlerInterface>())
			{
				TabWidgets.Add(TabIndex, InventoryTabWidget);
			}
			InventoryTabWidget->SetAbilitySystemComponent(*ASC);
		}
	}
	
	KY_LOG(LogKY, Log, TEXT("Created %d tab widgets"), TabWidgets.Num());
}

void UKYStatusWindowWidget::InitializeTabButtons()
{
	// 탭 버튼 컨테이너 초기화
	TabButtons.Empty();

	if (!TabButtonBox)
	{
		KY_LOG(LogKY, Warning, TEXT("Please Binding `TabButtonBox`"));
		return;
	}
	
	int32 TabIndex = 0;
	for (auto Widget: TabButtonBox->GetAllChildren())
	{
		if (UKYButtonWidget* TabButton = Cast<UKYButtonWidget>(Widget))
		{
			FButtonStyle CurrentStyle = TabButton->GetButtonStyle();
			TabButton->SetButtonIndex(TabIndex);
			TabButton->OnButtonClickedWithIndex.AddDynamic(this, &UKYStatusWindowWidget::OnTabButtonClicked);
			ButtonDefaultStyles.Add(TabButton, CurrentStyle);
			TabButtons.Add(TabButton);

			TabIndex++;
		}
	}
	KY_LOG(LogKY, Log, TEXT("Found %d tab buttons"), TabButtons.Num());
}



void UKYStatusWindowWidget::OnTabButtonClicked(int32 TabIndex)
{
	HandleTabSwitchInput_Implementation(TabIndex);
}


void UKYStatusWindowWidget::UpdateTabButtonStates(int32 InTabIndex)
{
	// 모든 탭 버튼 상태 업데이트
	for (int32 i = 0; i < TabButtons.Num(); ++i)
	{
		UKYButtonWidget* Button = TabButtons[i];
		if (Button)
		{
			if (InTabIndex == i)
			{
				// 선택된 탭 - Pressed 스타일 적용
				FButtonStyle PressedStyle = ButtonDefaultStyles[Button];
				PressedStyle.Normal = PressedStyle.Pressed;
				PressedStyle.Hovered = PressedStyle.Pressed;
				Button->SetButtonStyle(PressedStyle);
			}
			else
			{
				// 선택되지 않은 탭 - 일반 스타일 적용
				Button->SetButtonStyle(ButtonDefaultStyles[Button]);
			}
		}
	}
}

bool UKYStatusWindowWidget::HandleTabSwitchInput_Implementation(int32 TabIndex)
{
	if (!TabSwitcher) return false;
	
	// 유효한 탭 인덱스 확인
	if (TabIndex < 0 || TabIndex >= TabButtons.Num() || TabIndex == ActiveTabIndex ) return false;
	
	
	// 탭 인덱스에 따라 적절한 위젯으로 전환
	if (TabSwitcher->GetActiveWidgetIndex() != TabIndex)
	{
		TabSwitcher->SetActiveWidgetIndex(TabIndex);
		
		// 활성화 탭 인덱스 확인 및 적용
		if (TabWidgets.Contains(TabIndex))
		{
			ActiveTabWidget = TabWidgets[TabIndex];
		}
		
		// 현재 탭 인덱스 저장
		ActiveTabIndex = TabIndex;
		
		// 버튼 상태 업데이트
		UpdateTabButtonStates(TabIndex);
	}
	
	return true;
}

bool UKYStatusWindowWidget::HandleWindowInput_Implementation()
{
	if (ActiveTabWidget) return IKYInputHandlerInterface::Execute_HandleWindowInput(ActiveTabWidget);
	return false;
}


void UKYStatusWindowWidget::SwitchToNextTab()
{
	int32 NextTabIndex = ActiveTabIndex + 1;
	if (NextTabIndex >= TabButtons.Num())
	{
		NextTabIndex = 0; // 마지막 탭에서 첫 번째 탭으로 순환
	}
    
	HandleTabSwitchInput_Implementation(NextTabIndex);
}

void UKYStatusWindowWidget::SwitchToPreviousTab()
{
	KY_LOG(LogKY, Log, TEXT("ActiveTabIndex : %d"), ActiveTabIndex);
	int32 PrevTabIndex = ActiveTabIndex - 1;
	if (PrevTabIndex < 0)
	{
		PrevTabIndex = TabButtons.Num() - 1; // 첫 번째 탭에서 마지막 탭으로 순환
	}
	KY_LOG(LogKY, Log, TEXT("PrevTabIndex : %d"), PrevTabIndex);
	HandleTabSwitchInput_Implementation(PrevTabIndex);
}

bool UKYStatusWindowWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	bool Result = false;

	// 데드존 X
	if (abs(AxisX) > 0.5f)
	{
		if (ActiveTabWidget)
		{
			Result = IKYInputHandlerInterface::Execute_HandleNavigationInput(ActiveTabWidget, AxisX, AxisY);

			// 하위 탭 위젯에서 실행할 수 없을 때
			if(!Result)
			{

			}
		}
	}
	
	// 데드존 Y
	if (abs(AxisY) > 0.5f)
	{
		if (ActiveTabWidget)
		{
			Result = IKYInputHandlerInterface::Execute_HandleNavigationInput(ActiveTabWidget, AxisX, AxisY);

			// 하위 탭 위젯에서 실행할 수 없을 때
			if(!Result)
			{

			}
		}
	}
	
	return Result; 
}
