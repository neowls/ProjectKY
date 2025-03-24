// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Item/KYItem.h"

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
	InstanceID = NAME_None;
	ClearSlot();
}

void UKYItemSlotWidget::UpdateSlot(const FName& InInstanceID, const FKYItemData& ItemData)
{
	InstanceID = InInstanceID;
    
	// 아이템 아이콘 설정
	if (ItemIcon && ItemData.Icon.IsValid())
	{
		ItemIcon->SetBrushFromTexture(ItemData.Icon.LoadSynchronous());
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
 
	// 아이템 개수 표시 (중첩 가능한 경우)
	if (ItemCount)
	{
		if (ItemData.bIsStackable && ItemData.Count > 1)
		{
			ItemCount->SetText(FText::AsNumber(ItemData.Count));
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
		if (ItemData.ItemType != EKYItemType::Equipment)
		{
			EquippedBorder->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
		
		bool bIsEquipped = false;
		
		UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData.SubData);
		if (EquipItem)
		{
			bIsEquipped = EquipItem->bIsEquipped;
		}
		EquippedBorder->SetVisibility(bIsEquipped ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
    
	// 슬롯 버튼 활성화
	if (ItemButton)
	{
		ItemButton->SetIsEnabled(true);
	}
}

void UKYItemSlotWidget::ClearSlot()
{
	InstanceID = NAME_None;
    
	if (ItemIcon)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
    
	if (ItemCount)
	{
		ItemCount->SetVisibility(ESlateVisibility::Collapsed);
	}
    
	if (EquippedBorder)
	{
		EquippedBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
    
	if (SelectionBorder)
	{
		SelectionBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
    
	if (ItemButton)
	{
		ItemButton->SetIsEnabled(false);
	}
}

void UKYItemSlotWidget::SetIsSelected(bool bSelected)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}


void UKYItemSlotWidget::OnSlotButtonClicked()
{
	OnSlotClicked.Broadcast(this);
}
