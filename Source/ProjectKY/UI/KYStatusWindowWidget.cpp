
#include "UI/KYStatusWindowWidget.h"
#include "CommonButtonBase.h"
#include "CommonActivatableWidgetSwitcher.h"

UKYStatusWindowWidget::UKYStatusWindowWidget()
{
	ActiveTabIndex = 0;
}


void UKYStatusWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
    
	// 탭 버튼 클릭 이벤트 바인딩
	if (StatsTabButton)
	{
		StatsTabButton->OnClicked().AddUObject(this, &UKYStatusWindowWidget::OnStatsTabButtonClicked);
		TabButtons.Add(StatsTabButton);
	}
    
	if (SkillsTabButton)
	{
		SkillsTabButton->OnClicked().AddUObject(this, &UKYStatusWindowWidget::OnSkillsTabButtonClicked);
		TabButtons.Add(SkillsTabButton);
	}
    
	if (InventoryTabButton)
	{
		InventoryTabButton->OnClicked().AddUObject(this, &UKYStatusWindowWidget::OnInventoryTabButtonClicked);
		TabButtons.Add(InventoryTabButton);
	}

	// 탭 위젯 생성
	if (TabSwitcher)
	{
		if (StatsTabWidgetClass)
		{
			StatsTabWidget = CreateWidget<UCommonActivatableWidget>(this, StatsTabWidgetClass);
			if (StatsTabWidget)
			{
				TabSwitcher->AddChild(StatsTabWidget);
				
			}
		}
        
		if (SkillsTabWidgetClass)
		{
			SkillsTabWidget = CreateWidget<UCommonActivatableWidget>(this, SkillsTabWidgetClass);
			if (SkillsTabWidget)
			{
				TabSwitcher->AddChild(SkillsTabWidget);
			}
		}
        
		if (InventoryTabWidgetClass)
		{
			InventoryTabWidget = CreateWidget<UCommonActivatableWidget>(this, InventoryTabWidgetClass);
			if (InventoryTabWidget)
			{
				TabSwitcher->AddChild(InventoryTabWidget);
			}
		}
	}
}

void UKYStatusWindowWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
    
	// 위젯이 활성화될 때 마지막으로 선택된 탭 또는 기본 탭 표시
	SwitchToTab(ActiveTabIndex);
}

void UKYStatusWindowWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UKYStatusWindowWidget::OnStatsTabButtonClicked()
{
	SwitchToTab(0);
}

void UKYStatusWindowWidget::OnSkillsTabButtonClicked()
{
	SwitchToTab(1);
}

void UKYStatusWindowWidget::OnInventoryTabButtonClicked()
{
	SwitchToTab(2);
}

void UKYStatusWindowWidget::UpdateTabButtonStates(int32 InTabIndex)
{
	// 모든 탭 버튼 비활성화 상태로 설정
	for (int32 i = 0; i < TabButtons.Num(); ++i)
	{
		if (TabButtons[i])
		{
			TabButtons[i]->SetIsSelected(i == InTabIndex);
		}
	}
}


void UKYStatusWindowWidget::SwitchToTab(int32 TabIndex)
{
	if (!TabSwitcher) return;
	
	// 유효한 탭 인덱스 확인
	if (TabIndex < 0 || TabIndex >= TabButtons.Num()) return;
    
	// 현재 탭 인덱스 저장
	ActiveTabIndex = TabIndex;
    
	// 탭 인덱스에 따라 적절한 위젯으로 전환
	if (TabSwitcher->GetActiveWidgetIndex() != TabIndex)
	{
		TabSwitcher->SetActiveWidgetIndex(TabIndex);
	}
	
	// 버튼 상태 업데이트
	UpdateTabButtonStates(TabIndex);
}

void UKYStatusWindowWidget::SwitchToNextTab()
{
	int32 NextTabIndex = ActiveTabIndex + 1;
	if (NextTabIndex >= TabButtons.Num())
	{
		NextTabIndex = 0; // 마지막 탭에서 첫 번째 탭으로 순환
	}
    
	SwitchToTab(NextTabIndex);
}

void UKYStatusWindowWidget::SwitchToPreviousTab()
{
	int32 PrevTabIndex = ActiveTabIndex - 1;
	if (PrevTabIndex < 0)
	{
		PrevTabIndex = TabButtons.Num() - 1; // 첫 번째 탭에서 마지막 탭으로 순환
	}
    
	SwitchToTab(PrevTabIndex);
}
