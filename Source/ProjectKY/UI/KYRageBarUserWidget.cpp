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
	// 이전 상태 저장
	int32 PreviousCell = CurrentCell;
    
	// 현재 채워져야 하는 칸 수 계산
	CurrentCell = FMath::FloorToInt(CurrentRage / RagePerCell);
	float Remainder = CurrentRage - (CurrentCell * RagePerCell);
    
	// 변경된 셀만 업데이트
	for (int32 i = 0; i < MaxCell; i++)
	{
		// 이전과 현재 상태가 같으면 업데이트 건너뛰기
		bool bWasFilled = (i < PreviousCell);
		bool bIsFilled = (i < CurrentCell);
		bool bWasPartial = (i == PreviousCell);
		bool bIsPartial = (i == CurrentCell);
        
		// 상태가 변경된 경우만 업데이트
		if (bWasFilled != bIsFilled || bWasPartial != bIsPartial || bIsPartial)
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
}

void UKYRageBarUserWidget::CreateWidgets()
{
	if (!ProgressBarBox || !ImageBox)
	{
		KY_LOG(LogKY, Log, TEXT("Not Valid"));
		return;
	}
	
	// 기존 위젯 수와 필요한 위젯 수 비교
	int32 CurrentImageCount = ImageBox->GetChildrenCount();
	int32 CurrentProgressCount = ProgressBarBox->GetChildrenCount();
    
	// 필요한 만큼만 위젯 생성/제거
	if (CurrentImageCount > MaxCell)
	{
		// 초과 위젯 제거
		for (int32 i = CurrentImageCount - 1; i >= MaxCell; --i)
		{
			ImageBox->RemoveChildAt(i);
		}
	}
	else if (CurrentImageCount < MaxCell)
	{
		// 부족한 위젯 추가
		for (int32 i = CurrentImageCount; i < MaxCell; ++i)
		{
			UImage* ImageWidget = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			if (ImageWidget)
			{
				ImageWidget->SetBrush(ImageBrush);
				ImageWidget->SetVisibility(ESlateVisibility::Hidden);
				ImageBox->AddChildToHorizontalBox(ImageWidget);
			}
		}
	}
    
	// ProgressBar도 동일한 방식으로 처리
	if (CurrentProgressCount > MaxCell)
	{
		for (int32 i = CurrentProgressCount - 1; i >= MaxCell; --i)
		{
			ProgressBarBox->RemoveChildAt(i);
		}
	}
	else if (CurrentProgressCount < MaxCell)
	{
		for (int32 i = CurrentProgressCount; i < MaxCell; ++i)
		{
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
}
