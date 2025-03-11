// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "UI/KYUserWidget.h"
#include "KYAttributeTextWidget.generated.h"

class UTextBlock;

UENUM(BlueprintType)
enum class EAttributeValueType : uint8
{
	Integer,
	Float,
	Percentage
};


UCLASS()
class PROJECTKY_API UKYAttributeTextWidget : public UKYUserWidget
{
	GENERATED_BODY()
	
public:
	// 어트리뷰트 설정 함수
	void SetAttribute(const FGameplayAttribute& InAttribute, EAttributeValueType InValueType = EAttributeValueType::Integer);
    
	// 어트리뷰트 값 업데이트
	void UpdateAttributeValue();
    
	// 어트리뷰트 이름 설정
	UFUNCTION(BlueprintCallable)
	void SetAttributeName(const FText& InName);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
    
	// 어트리뷰트 변경 이벤트 핸들러
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
        
	// 어트리뷰트 이름 포맷팅 함수
	void FormatAttributeName(FString& AttributeName);
	
	// 어트리뷰트 이름 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttributeNameText;
    
	// 어트리뷰트 값 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttributeValueText;
    
	// 어트리뷰트 정보
	UPROPERTY()
	FGameplayAttribute Attribute;

	// 어트리뷰트 값 타입
	UPROPERTY()
	EAttributeValueType ValueType;
    
	// 델리게이트 핸들
	FDelegateHandle AttributeChangedHandle;
};
