// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Interface/KYInputHandlerInterface.h"
#include "Struct/KYStruct.h"
#include "KYAbilitiesTabWidget.generated.h"


class UKYButtonWidget;
class UScrollBox;
class UTextBlock;
class UKYVideoWidget;

UCLASS()
class PROJECTKY_API UKYAbilitiesTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	UKYAbilitiesTabWidget();
    
    virtual void NativeConstruct() override;
    
    // 어빌리티 데이터 설정
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetAbilityData(const TArray<FKYAbilityData>& InAbilityDataArray);

	// 인터페이스 구현
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	
	bool HandleHorizontalNavigation(float AxisX);
	bool HandleVerticalNavigation(float AxisY);
	bool HandleCategoryNavigation(float AxisY);
	bool HandleAbilityNavigation(float AxisY);

protected:
    // 위젯 초기화
	void CheckInitializedWidgets() const;

	void ClearAbilityDetails();

	void CreateAbilityButtons(const TArray<FKYAbilityData>& Abilities);

	//	카테고리 목록 초기화
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitializeCategoryList();

	//	선택된 카테고리로 어빌리티 목록 변경
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void UpdateAbilityList(const EKYAbilityCategory Category);

	// 선택된 카테고리 변경
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ChangeSelectedCategory(int32 NewIndex);
    
	// 선택된 어빌리티 변경
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ChangeSelectedAbility(int32 NewIndex);
	
    // 어빌리티 세부 정보 업데이트
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void UpdateAbilityDetails();
	
	// 카테고리 버튼 클릭 이벤트 핸들러
	UFUNCTION()
	void OnCategoryButtonClicked(int32 ButtonIndex);
	
    // 어빌리티 버튼 클릭 이벤트 핸들러
    UFUNCTION()
    void OnAbilityButtonClicked(int32 ButtonIndex);

	
	// 카테고리 스크롤 박스 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> CategoryScrollBox;
    
	// 어빌리티 스크롤 박스 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> AbilityScrollBox;
    
	// 비디오 위젯 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKYVideoWidget> AbilityVideoWidget;
    
	// 설명 텍스트 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityDescriptionText;
    
	// 커맨드 텍스트 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityCommandText;
    
	// 어빌리티 이름 텍스트 (바인딩)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AbilityNameText;
	
    // 버튼 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UKYButtonWidget> CategoryButtonWidgetClass;

	// 버튼 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKYButtonWidget> AbilityButtonWidgetClass;
    
	// 카테고리 버튼 위젯 배열
	UPROPERTY()
	TArray<TObjectPtr<class UKYButtonWidget>> CategoryButtonWidgets;
    
	// 어빌리티 버튼 위젯 배열
	UPROPERTY()
	TArray<TObjectPtr<class UKYButtonWidget>> AbilityButtonWidgets;
    
	// 카테고리별 어빌리티 맵
	TMap<EKYAbilityCategory, TArray<FKYAbilityData>> CategoryAbilityMap;

	// 어빌리티 셋
	UPROPERTY()
	TArray<FKYAbilityData> AbilityArray;

	// 선택된 카테고리 인덱스
	UPROPERTY()
	int32 SelectedCategoryIndex;
    
	// 선택된 어빌리티 인덱스
	UPROPERTY()
	int32 SelectedAbilityIndex;
    
	// 현재 선택된 카테고리
	UPROPERTY()
	EKYAbilityCategory CurrentCategory;

	// 네비게이션 모드 (0: 카테고리, 1: 어빌리티)
	UPROPERTY()
	int32 NavigationMode;

	UPROPERTY()
	TArray<EKYAbilityCategory> AllCategories;
};


