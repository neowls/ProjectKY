#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Interface/KYInputHandlerInterface.h"
#include "KYStatusWindowWidget.generated.h"

class UKYAbilitiesTabWidget;
class UKYStatsTabWidget;
class UKYButtonWidget;

UCLASS()
class PROJECTKY_API UKYStatusWindowWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()
	
public:
	UKYStatusWindowWidget();

	UFUNCTION(BlueprintCallable)
	void ActivateWindow();

	UFUNCTION(BlueprintCallable)
	void DeactivateWindow();
	
	UFUNCTION(BlueprintCallable)
	void SwitchToNextTab();

	UFUNCTION(BlueprintCallable)
	void SwitchToPreviousTab();
	
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	virtual bool HandleTabSwitchInput_Implementation(int32 TabIndex) override;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTabButtonClicked(int32 TabIndex);
	void UpdateTabButtonStates(int32 InTabIndex);

	// 탭 위젯 초기화
	void InitializeTabWidgets();
    
	// 탭 버튼 초기화
	void InitializeTabButtons();

protected:
	// 탭 컨테이너(스위처)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> TabSwitcher;
    
	// 탭 버튼 컨테이너(수평 박스)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> TabButtonBox;

	// 현재 활성화된 탭 인덱스
	UPROPERTY()
	int32 ActiveTabIndex;

	// 현재 활성화된 탭
	UPROPERTY()
	TObjectPtr<UKYUserWidget> ActiveTabWidget;

	// 탭 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYUserWidget> StatsTabWidgetClass;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYUserWidget> AbilitiesTabWidgetClass;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYUserWidget> InventoryTabWidgetClass;
    
	// 탭 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<UKYStatsTabWidget> StatsTabWidget;
    
	UPROPERTY()
	TObjectPtr<UKYAbilitiesTabWidget> AbilitiesTabWidget;
    
	UPROPERTY()
	TObjectPtr<UKYUserWidget> InventoryTabWidget;

	//	탭 버튼 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYButtonWidget> TabButtonWidgetClass;
	
	// 탭 버튼 배열
	UPROPERTY()
	TArray<TObjectPtr<UKYButtonWidget>> TabButtons;

	// 탭 위젯 맵
	UPROPERTY()
	TMap<int32, TObjectPtr<UKYUserWidget>> TabWidgets;
    
	// 버튼 스타일 저장
	UPROPERTY()
	TMap<TObjectPtr<UKYButtonWidget>, FButtonStyle> ButtonDefaultStyles;
};

