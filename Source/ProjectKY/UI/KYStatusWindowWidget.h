#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "KYStatusWindowWidget.generated.h"

class UCommonButtonBase;

UCLASS()
class PROJECTKY_API UKYStatusWindowWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UKYStatusWindowWidget();
	
	// 탭 전환 함수
	UFUNCTION(BlueprintCallable)
	void SwitchToTab(int32 TabIndex);

	UFUNCTION(BlueprintCallable)
	void SwitchToNextTab();

	UFUNCTION(BlueprintCallable)
	void SwitchToPreviousTab();
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	// 탭 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnStatsTabButtonClicked();
    
	UFUNCTION()
	void OnSkillsTabButtonClicked();
    
	UFUNCTION()
	void OnInventoryTabButtonClicked();
    
	// 탭 버튼 상태 업데이트
	void UpdateTabButtonStates(int32 InTabIndex);

protected:
	// 탭 스위처
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonActivatableWidgetSwitcher> TabSwitcher;
    
	// 탭 버튼들
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> StatsTabButton;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> SkillsTabButton;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonButtonBase> InventoryTabButton;

	// 현재 활성화된 탭 인덱스
	UPROPERTY()
	int32 ActiveTabIndex;

	// 탭 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> StatsTabWidgetClass;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> SkillsTabWidgetClass;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonActivatableWidget> InventoryTabWidgetClass;
    
	// 탭 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> StatsTabWidget;
    
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> SkillsTabWidget;
    
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> InventoryTabWidget;
    
	// 탭 버튼 배열 (쉬운 탐색을 위해)
	UPROPERTY()
	TArray<TObjectPtr<UCommonButtonBase>> TabButtons;
};

