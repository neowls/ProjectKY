// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "AbilitySystemComponent.h"
#include "KYAttributeTextWidget.h"
#include "KYStatsTabWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTKY_API UKYStatsTabWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
    
	// 어트리뷰트 위젯 컨테이너
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> AttributesContainer;
    
	// 어트리뷰트 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKYAttributeTextWidget> AttributeTextWidgetClass;
    
	// 어트리뷰트 위젯 생성 및 설정
	void SetupAttributeWidgets();
    
	// 카테고리 헤더 추가 함수
	void AddCategoryHeader(const FString& CategoryName);
    
	// 어트리뷰트 위젯 추가 함수
	void AddAttributeWidget(FGameplayAttribute Attribute, UAbilitySystemComponent* ASC, EAttributeValueType ValueType = EAttributeValueType::Integer);
    
	// 어트리뷰트 위젯 인스턴스
	UPROPERTY()
	TArray<TObjectPtr<UKYAttributeTextWidget>> AttributeWidgets;
};