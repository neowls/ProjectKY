// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYStatsTabWidget.h"
#include "UI/KYAttributeTextWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CommonInputSubsystem.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "Character/KYCharacterPlayer.h"


void UKYStatsTabWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UKYStatsTabWidget::SetupAttributeWidgets()
{
    if (!AttributesContainer || !AttributeTextWidgetClass) return;
    
    // 컨테이너 초기화
    AttributesContainer->ClearChildren();
    AttributeWidgets.Empty();
    
    if (!ASC) return;
    
    // 카테고리 헤더 추가 - 기본 스탯
    AddCategoryHeader(TEXT("Base"));
    
    // 기본 스탯 어트리뷰트 위젯 생성
    AddAttributeWidget(UKYAttributeSetBase::GetLevelAttribute(),  EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetHealthAttribute(),  EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetMaxHealthAttribute(),  EAttributeValueType::Integer);
    
    // 카테고리 헤더 추가 - 전투 스탯
    AddCategoryHeader(TEXT("Combat"));
    
    // 전투 스탯 어트리뷰트 위젯 생성
    AddAttributeWidget(UKYAttributeSetBase::GetStrikingPowerAttribute(),  EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetDefensivePowerAttribute(),  EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetAttackSpeedAttribute(),  EAttributeValueType::Float);
    AddAttributeWidget(UKYAttributeSetBase::GetMoveSpeedAttribute(),  EAttributeValueType::Float);
    
    // 플레이어 전용 어트리뷰트 위젯 생성
    if (const UKYAttributeSetPlayer* PlayerAttributes = ASC->GetSet<UKYAttributeSetPlayer>())
    {
        // 카테고리 헤더 추가 - 플레이어 고유 스탯
        AddCategoryHeader(TEXT("Misc"));
        
        AddAttributeWidget(UKYAttributeSetPlayer::GetRageAttribute(),  EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetMaxRageAttribute(),  EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetExperienceAttribute(),  EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetMaxExperienceAttribute(),  EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetGoldAttribute(),  EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetSkillPointAttribute(),  EAttributeValueType::Integer);
    }
}

void UKYStatsTabWidget::AddCategoryHeader(const FString& InCategoryName)
{

    // 카테고리 헤더 위젯 생성 및 추가
    UTextBlock* HeaderText = NewObject<UTextBlock>(this);
    if (HeaderText)
    {
        HeaderText->SetText(FText::FromString(InCategoryName));
        HeaderText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.4f, 0.2f)));
        
        FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Bold", 32);
        HeaderText->SetFont(FontInfo);
        
        AttributesContainer->AddChild(HeaderText);
    }
}

void UKYStatsTabWidget::AddAttributeWidget(const FGameplayAttribute& Attribute, EAttributeValueType ValueType)
{
    // 메타 어트리뷰트는 표시하지 않음
    FString AttributeName = Attribute.GetName();
    if (AttributeName.StartsWith("In") && !AttributeName.Contains("Intelligence"))
    {
        return;
    }
    
    UKYAttributeTextWidget* AttributeWidget = CreateWidget<UKYAttributeTextWidget>(this, AttributeTextWidgetClass);
    if (AttributeWidget)
    {
        AttributeWidget->SetAbilitySystemComponent(*ASC);
        AttributeWidget->SetAttribute(Attribute, ValueType);
        AttributesContainer->AddChild(AttributeWidget);
        AttributeWidgets.Add(AttributeWidget);
    }
}