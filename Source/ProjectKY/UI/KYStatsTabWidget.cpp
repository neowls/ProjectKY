// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYStatsTabWidget.h"
#include "UI/KYAttributeTextWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Attribute/KYAttributeSetBase.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"
#include "Character/KYCharacterPlayer.h"


void UKYStatsTabWidget::NativeOnActivated()
{
    Super::NativeOnActivated();
    
    // 어트리뷰트 위젯 설정
    SetupAttributeWidgets();
}

void UKYStatsTabWidget::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
    
    // 어트리뷰트 위젯 정리
    AttributeWidgets.Empty();
    if (AttributesContainer)
    {
        AttributesContainer->ClearChildren();
    }
}

void UKYStatsTabWidget::SetupAttributeWidgets()
{
    if (!AttributesContainer || !AttributeTextWidgetClass) return;
    
    // 컨테이너 초기화
    AttributesContainer->ClearChildren();
    AttributeWidgets.Empty();
    
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;
    
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
    if (!ASC) return;
    
    // 카테고리 헤더 추가 - 기본 스탯
    AddCategoryHeader(TEXT("Basic Stats"));
    
    // 기본 스탯 어트리뷰트 위젯 생성
    AddAttributeWidget(UKYAttributeSetBase::GetLevelAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetHealthAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetMaxHealthAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetShieldAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetMaxShieldAttribute(), ASC, EAttributeValueType::Integer);
    
    // 카테고리 헤더 추가 - 전투 스탯
    AddCategoryHeader(TEXT("Combat Stats"));
    
    // 전투 스탯 어트리뷰트 위젯 생성
    AddAttributeWidget(UKYAttributeSetBase::GetStrikingPowerAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetDefensivePowerAttribute(), ASC, EAttributeValueType::Integer);
    AddAttributeWidget(UKYAttributeSetBase::GetAttackSpeedAttribute(), ASC, EAttributeValueType::Float);
    AddAttributeWidget(UKYAttributeSetBase::GetMoveSpeedAttribute(), ASC, EAttributeValueType::Float);
    
    // 플레이어 전용 어트리뷰트 위젯 생성
    if (const UKYAttributeSetPlayer* PlayerAttributes = ASC->GetSet<UKYAttributeSetPlayer>())
    {
        // 카테고리 헤더 추가 - 플레이어 스탯
        AddCategoryHeader(TEXT("Player Stats"));
        
        AddAttributeWidget(UKYAttributeSetPlayer::GetRageAttribute(), ASC, EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetMaxRageAttribute(), ASC, EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetExperienceAttribute(), ASC, EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetMaxExperienceAttribute(), ASC, EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetGoldAttribute(), ASC, EAttributeValueType::Integer);
        AddAttributeWidget(UKYAttributeSetPlayer::GetSkillPointAttribute(), ASC, EAttributeValueType::Integer);
    }
}

void UKYStatsTabWidget::AddCategoryHeader(const FString& CategoryName)
{
    // 카테고리 헤더 위젯 생성 및 추가
    UTextBlock* HeaderText = NewObject<UTextBlock>(this);
    if (HeaderText)
    {
        HeaderText->SetText(FText::FromString(CategoryName));
        HeaderText->SetColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.8f, 1.0f)));
        
        FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Bold", 16);
        HeaderText->SetFont(FontInfo);
        
        AttributesContainer->AddChild(HeaderText);
    }
}

void UKYStatsTabWidget::AddAttributeWidget(FGameplayAttribute Attribute, UAbilitySystemComponent* ASC, EAttributeValueType ValueType)
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
        AttributeWidget->SetAbilitySystemComponent(ASC->GetOwnerActor());
        AttributeWidget->SetAttribute(Attribute, ValueType);
        AttributesContainer->AddChild(AttributeWidget);
        AttributeWidgets.Add(AttributeWidget);
    }
}