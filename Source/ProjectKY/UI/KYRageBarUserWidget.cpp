// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYRageBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "ProjectKY.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "GAS/Attribute/KYAttributeSetPlayer.h"

UKYRageBarUserWidget::UKYRageBarUserWidget()
{
	ImageBrush = FSlateBrush();
	ImageBrush.ImageSize.Y = 16.0f;
	ImageBrush.TintColor = FSlateColor(FColor(0.0f, 0.7f, 1.0f, 1.0f));

	ProgressBarStyle.SetBackgroundImage(FSlateBrush());
	ProgressBarStyle.BackgroundImage.DrawAs = ESlateBrushDrawType::Type::NoDrawType;
	ProgressBarStyle.FillImage.TintColor = FSlateColor(FColor(0.0f, 0.25f, 0.5f, 1.0f));
}

void UKYRageBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	CreateWidgets();
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetPlayer::GetRageAttribute()).AddUObject(this, &UKYRageBarUserWidget::OnRageChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UKYAttributeSetPlayer::GetMaxRageAttribute()).AddUObject(this, &UKYRageBarUserWidget::OnMaxRageChanged);
		
		const UKYAttributeSetPlayer* AttributeSetPlayer = ASC->GetSet<UKYAttributeSetPlayer>();
		if (AttributeSetPlayer)
		{
			CurrentRage = AttributeSetPlayer->GetRage();
			CurrentMaxRage = AttributeSetPlayer->GetMaxRage();
			RagePerCell = 20.0f;
			CurrentCell = 0;
			MaxCell = CurrentMaxRage / RagePerCell;
			
			if (CurrentMaxRage > 0.0f)
			{
				UpdateRageBar();
			}
		}
		else
		{
			KY_LOG(LogKY, Log, TEXT("Can't not Load AttributeSet"));
		}
	}
}

void UKYRageBarUserWidget::OnRageChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentRage = ChangeData.NewValue;
	UpdateRageBar();
}

void UKYRageBarUserWidget::OnMaxRageChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxRage = ChangeData.NewValue;
	CreateWidgets();
	UpdateRageBar();

	KY_LOG(LogKY, Log, TEXT("Max Rage: %f"), CurrentMaxRage);
}

void UKYRageBarUserWidget::UpdateRageBar()
{
	// 현재 채워져야 하는 칸 수 계산
	CurrentCell = FMath::FloorToInt(CurrentRage / RagePerCell);
	float Remainder = CurrentRage - (CurrentCell * RagePerCell);
	
	// 모든 칸 업데이트
	for (int32 i = 0; i < MaxCell; i++)
	{
		if (i < CurrentCell)
		{
			// 완전히 채워진 칸
			ImageBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Visible);
			ProgressBarBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Visible);
		}
		else if (i == CurrentCell)
		{
			// 현재 채워지고 있는 칸
			ImageBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
			ProgressBarBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Visible);
			KY_LOG(LogKY, Log, TEXT("Rage Percentage : %f"), Remainder / RagePerCell);
			Cast<UProgressBar>(ProgressBarBox->GetChildAt(i))->SetPercent(Remainder / RagePerCell);
		}
		else
		{
			// 아직 채워지지 않은 칸
			ImageBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Hidden);
			Cast<UProgressBar>(ProgressBarBox->GetChildAt(i))->SetPercent(0.0f);
			
		}
	}
}

void UKYRageBarUserWidget::CreateWidgets()
{
	if (!ProgressBarBox || !ImageBox)
	{
		KY_LOG(LogKY, Log, TEXT("Not Valid"));
		return;
	}
	
	ProgressBarBox->ClearChildren();
	ImageBox->ClearChildren();

	KY_LOG(LogKY, Log, TEXT("MaxCell : %d"), MaxCell);

	for (int32 i = 0; i < MaxCell; i++)
	{
		UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (ImageWidget)
		{
			ImageWidget->SetBrush(ImageBrush);
			ImageWidget->SetVisibility(ESlateVisibility::Hidden);
			ImageBox->AddChildToHorizontalBox(ImageWidget);
		}

		UProgressBar* ProgressBarWidget = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
		if (ProgressBarWidget)
		{
			ProgressBarWidget->SetWidgetStyle(ProgressBarStyle);
			ProgressBarWidget->SetVisibility(ESlateVisibility::Visible);
			ProgressBarWidget->SetPercent(0.0f);
			ProgressBarBox->AddChildToHorizontalBox(ProgressBarWidget);
		}
	}
}
