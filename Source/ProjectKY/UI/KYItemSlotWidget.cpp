// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KYItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UKYItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	    
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UKYItemSlotWidget::HandleButtonClicked);
	}
}

void UKYItemSlotWidget::SetItemData(const FKYItemData& NewItemData, EKYSlotType InType, int32 InIndex)
{
	ItemData = NewItemData;
	SlotType = InType;
	SlotIndex = InIndex;
	UpdateVisuals();
}

void UKYItemSlotWidget::ClearSlot()
{
	ItemData = FKYItemData();
	bIsSelected = false;
	UpdateVisuals();
}

void UKYItemSlotWidget::SetIsSelected(bool NewSelect)
{
	if (bIsSelected != NewSelect)
	{
		bIsSelected = NewSelect;
		UpdateVisuals();
	}
}

void UKYItemSlotWidget::HandleButtonClicked()
{
	OnClicked.Broadcast(this);
}

void UKYItemSlotWidget::UpdateVisuals()
{
	if (ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemData.ItemIcon);
		ItemIcon->SetVisibility(ItemData.ItemIcon ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
