// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "UI/KYItemSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"



void UKYInventoryTabWidget::OnSlotClicked(UKYItemSlotWidget* InSlot)
{
	// 이전 선택 해제
	if (SelectedSlot && SelectedSlot != InSlot)
	{
		SelectedSlot->SetIsSelected(false);
	}
    
	// 새로운 슬롯 선택
	SelectedSlot = InSlot;
	if (SelectedSlot)
	{
		SelectedSlot->SetIsSelected(true);
		UpdateDetailPanel(SelectedSlot->GetItemData());
	}
	else
	{
		ClearDetailPanel();
	}
}

bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	if (!SelectedSlot)
	{
		// 아무것도 선택되지 않은 경우 첫 번째 슬롯 선택
		if (AllSlots.Num() > 0)
		{
			OnSlotClicked(AllSlots.CreateIterator().Value());
			return true;
		}
		return false;
	}
    
	// 현재 선택된 슬롯의 인덱스 기준으로 이동
	int32 CurrentIndex = SelectedSlot->GetIndex();
	int32 NewIndex = CurrentIndex;
    
	if (FMath::Abs(AxisX) > 0.5f)
	{
		NewIndex += (AxisX > 0) ? 1 : -1;
	}
	else if (FMath::Abs(AxisY) > 0.5f)
	{
		NewIndex += (AxisY > 0) ? GridColumns : -GridColumns;
	}
    
	// 새 인덱스가 유효한지 확인
	if (AllSlots.Contains(EKYSlotType::Inventory))
	{
		if (UKYItemSlotWidget* NewSlot = AllSlots[EKYSlotType::Inventory])
		{
			if (NewSlot->GetIndex() == NewIndex)
			{
				OnSlotClicked(NewSlot);
				return true;
			}
		}
	}
    
	return false;
}

void UKYInventoryTabWidget::UpdateDetailPanel(const FKYItemData& ItemData)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemData.ItemName);
	}
    
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(ItemData.ItemDescription);
	}
    
	if (DetailItemIcon)
	{
		if (ItemData.ItemIcon)
		{
			DetailItemIcon->SetBrushFromTexture(ItemData.ItemIcon);
			DetailItemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DetailItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UKYInventoryTabWidget::ClearDetailPanel()
{
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::GetEmpty());
	}
    
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(FText::GetEmpty());
	}
    
	if (DetailItemIcon)
	{
		DetailItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}
