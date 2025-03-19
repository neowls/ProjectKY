// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Interface/KYInputHandlerInterface.h"
#include "UI/KYAttributeTextWidget.h"
#include "UI/KYUserWidget.h"
#include "KYStatsTabWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTKY_API UKYStatsTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	// 어트리뷰트 위젯 생성 및 설정
	UFUNCTION(BlueprintCallable)
	void SetupAttributeWidgets();

protected:
	virtual void NativeConstruct() override;
    
	// 어트리뷰트 위젯 컨테이너
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> AttributesContainer;
    
	// 어트리뷰트 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UKYAttributeTextWidget> AttributeTextWidgetClass;
    
	// 카테고리 헤더 추가 함수
	void AddCategoryHeader(const FString& InCategoryName);
    
	// 어트리뷰트 위젯 추가 함수
	void AddAttributeWidget(const FGameplayAttribute& Attribute, EAttributeValueType ValueType = EAttributeValueType::Integer);
    
	// 어트리뷰트 위젯 인스턴스
	UPROPERTY()
	TArray<TObjectPtr<UKYAttributeTextWidget>> AttributeWidgets;
};