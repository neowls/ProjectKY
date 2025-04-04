// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UKYItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UKYItemSlotWidget::OnSlotButtonClicked);
	}

	ClearSlot();
}

void UKYItemSlotWidget::InitializeSlot()
{
	ClearSlot();
}

void UKYItemSlotWidget::UpdateSlot(const FKYInventoryWidgetData NewSlotData)
{
	SlotData = NewSlotData;
    
	// 아이템 아이콘 설정
	if (ItemIcon && NewSlotData.Icon)
	{
		ItemIcon->SetBrushFromTexture(NewSlotData.Icon);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
 
	// 아이템 개수 표시 (중첩 가능한 경우)
	if (ItemCount)
	{
		if (NewSlotData.Count > 1)
		{
			ItemCount->SetText(FText::AsNumber(NewSlotData.Count));
			
			ItemCount->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemCount->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
    
	// 장착 아이콘 표시
	if (EquippedBorder)
	{
		EquippedBorder->SetVisibility(NewSlotData.EquipState != EKYEquipmentState::Inventory ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
    
	// 슬롯 버튼 활성화
	if (ItemButton)
	{
		ItemButton->SetIsEnabled(true);
	}
}

void UKYItemSlotWidget::ClearSlot()
{
	SlotData.ClearData();
    
	if (ItemButton)
	{
		ItemButton->SetIsEnabled(false);
	}
}

void UKYItemSlotWidget::SetIsSelected(bool bSelected)
{
	bIsSelected = bSelected;
}


void UKYItemSlotWidget::OnSlotButtonClicked()
{
	OnSlotClicked.Broadcast(this);
}

FText UKYItemSlotWidget::GetItemCountText() const
{
	return FText::AsNumber(SlotData.Count);
}

FSlateBrush UKYItemSlotWidget::GetItemIconBrush() const
{
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Type::NoDrawType;
	Brush.ImageSize = FVector2D(128, 128);
	
	if (SlotData.Icon)
	{
		Brush.DrawAs = ESlateBrushDrawType::Type::Image;
		Brush.SetResourceObject(SlotData.Icon);
		FIntPoint ImportedSize = SlotData.Icon->GetImportedSize();
	
		const FVector2D FinalSize = ImportedSize.GetMin() > 0 ? FVector2D(ImportedSize) : FVector2D(64, 64);
		Brush.ImageSize = FinalSize;
	}

	return Brush;
}

ESlateVisibility UKYItemSlotWidget::GetItemCountVisibility() const
{
	return SlotData.Count > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
}

ESlateVisibility UKYItemSlotWidget::GetSelectionBorderVisibility() const
{
	return bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
}

ESlateVisibility UKYItemSlotWidget::GetEquippedBorderVisibility() const
{
	return SlotData.EquipState == EKYEquipmentState::Inventory ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
}
