// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYButtonWidget.h"
#include "ProjectKY.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UKYButtonWidget::UKYButtonWidget()
{
	ButtonIndex = 0;
	bIsSelected = false;
	DefaultButtonText = FText::GetEmpty();
}

void UKYButtonWidget::HandleButtonClicked()
{
	if(OnButtonClickedWithIndex.IsBound())	OnButtonClickedWithIndex.Broadcast(ButtonIndex);
}

void UKYButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 클릭 이벤트 바인딩
	if(!Button)
	{
		KY_LOG(LogKY, Warning, TEXT("Button Widget Not Valid."));
		return;
	}
	Button->OnClicked.AddDynamic(this, &UKYButtonWidget::HandleButtonClicked);
	DefaultButtonStyle = Button->GetStyle();
}

void UKYButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (DefaultButtonText.IsEmpty()) TextBlock->SetText(FText::FromName(*this->GetName()));
	else TextBlock->SetText(DefaultButtonText);
}

void UKYButtonWidget::SetButtonIndex(int32 InIndex)
{
	ButtonIndex = InIndex;
}

void UKYButtonWidget::SetIsSelected(bool bInIsSelected)
{
	bIsSelected = bInIsSelected;
	if (bIsSelected)
	{
		FButtonStyle Style = Button->GetStyle();
		Style.Normal = Style.Pressed;
		Style.Hovered = Style.Pressed;
		Button->SetStyle(Style);
		Button->SetIsEnabled(false);
	}
	else
	{
		Button->SetStyle(DefaultButtonStyle);
		Button->SetIsEnabled(true);
	}
}

void UKYButtonWidget::SetButtonText(const FText& InText)
{
	if (!TextBlock)
	{
		KY_LOG(LogKY, Warning, TEXT("Text Widget Not Valid."));
		return;
	}
	TextBlock->SetText(InText);
}

void UKYButtonWidget::SetButtonStyle(const FButtonStyle& InStyle)
{
	Button->SetStyle(InStyle);
}
