// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYAttributeTextWidget.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"

void UKYAttributeTextWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UKYAttributeTextWidget::NativeDestruct()
{
    // 델리게이트 핸들 제거
    if (ASC && AttributeChangedHandle.IsValid())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(AttributeChangedHandle);
        AttributeChangedHandle.Reset();
    }
    
    Super::NativeDestruct();
}

void UKYAttributeTextWidget::SetAttribute(const FGameplayAttribute& InAttribute, EAttributeValueType InValueType)
{
    // 기존 델리게이트 핸들 제거
    if (ASC && AttributeChangedHandle.IsValid())
    {
        ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(AttributeChangedHandle);
        AttributeChangedHandle.Reset();
    }
    
    // 새 어트리뷰트 및 값 타입 설정
    Attribute = InAttribute;
    ValueType = InValueType;
    
    // 어트리뷰트 이름 자동 설정
    if (AttributeNameText && Attribute.IsValid())
    {
        // 어트리뷰트 이름 가져오기
        FString AttributeName = Attribute.GetName();
        
        // 이름 포맷팅 - 대문자 기준으로 띄어쓰기 추가
        FormatAttributeName(AttributeName);
        
        // 텍스트 설정
        SetAttributeName(FText::FromString(AttributeName));
    }
    
    // 새 델리게이트 핸들 추가
    if (ASC && Attribute.IsValid())
    {
        AttributeChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UKYAttributeTextWidget::OnAttributeChanged);
        
        // 초기 값 업데이트
        UpdateAttributeValue();
    }
}
void UKYAttributeTextWidget::SetAttributeName(const FText& InName)
{
    if (AttributeNameText)
    {
        AttributeNameText->SetText(InName);
    }
}

void UKYAttributeTextWidget::UpdateAttributeValue()
{
    if (!ASC || !Attribute.IsValid() || !AttributeValueText) return;
    
    float Value = ASC->GetNumericAttribute(Attribute);
    
    // 값 타입에 따라 포맷 적용
    switch (ValueType)
    {
    case EAttributeValueType::Integer:
        // 정수 값으로 표시
            AttributeValueText->SetText(FText::AsNumber(FMath::FloorToInt(Value)));
        break;
        
    case EAttributeValueType::Float:
        {
            // 소수점 한 자리까지 표시
            FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
            Options.SetMinimumFractionalDigits(1);
            Options.SetMaximumFractionalDigits(1);
            AttributeValueText->SetText(FText::AsNumber(Value, &Options));
        }
        break;
        
    case EAttributeValueType::Percentage:
        {
            // 백분율로 표시 (100을 곱함)
            FNumberFormattingOptions Options = FNumberFormattingOptions::DefaultNoGrouping();
            Options.SetMinimumFractionalDigits(0);
            Options.SetMaximumFractionalDigits(0);
            AttributeValueText->SetText(FText::Format(NSLOCTEXT("Namespace", "Key", "{0}%"), FText::AsNumber(Value * 100.0f, &Options)));
        }
        break;
    }
}

void UKYAttributeTextWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
    UpdateAttributeValue();
}

void UKYAttributeTextWidget::FormatAttributeName(FString& AttributeName)
{
    // 대문자를 기준으로 띄어쓰기 추가
    FString FormattedName;
    bool bFirstChar = true;
    
    for (int32 i = 0; i < AttributeName.Len(); ++i)
    {
        TCHAR CurrentChar = AttributeName[i];
        
        // 첫 글자가 아니고 대문자인 경우 앞에 공백 추가
        if (!bFirstChar && FChar::IsUpper(CurrentChar))
        {
            FormattedName.AppendChar(' ');
        }
        
        FormattedName.AppendChar(CurrentChar);
        bFirstChar = false;
    }
    
    // 메타 어트리뷰트는 표시하지 않음 (InDamage, InRage, InExperience, IncomingStance)
    if (AttributeName.StartsWith("In") && !AttributeName.Contains("Intelligence"))
    {
        FormattedName = "META"; // 메타 어트리뷰트 표시
    }
    
    AttributeName = FormattedName;
}