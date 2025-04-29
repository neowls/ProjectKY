// Fill out your copyright notice in the Description page of Project Settings.


#include "KYItemSlotWidget.h"

#include "ProjectKY.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Data/KYInventoryItemObject.h"

void UKYItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ClearSlot();
}

void UKYItemSlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	const auto* Wrapper = Cast<UKYInventoryItemObject>(ListItemObject);
	if (!Wrapper) return;

	if (Wrapper->IsEmpty())
	{
		ItemIcon->SetBrush(FSlateBrush());
		ItemCount->SetText(FText::GetEmpty());
		SelectionImage->SetVisibility(ESlateVisibility::Hidden);
		bIsEmpty = true;
		return;
	}

	const auto Data =  Wrapper->GetItemPinned();
	if (!Data.IsValid()) return;
	
	ItemIcon->SetBrush(GetItemIconBrush(Data));
	ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (Data->InstanceData->Count > 1)
	{
		ItemCount->SetText(FText::AsNumber(Data->InstanceData->Count));
		ItemCount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
	UListView* OwningList = Cast<UListView>(GetOwningListView());
	if (OwningList && OwningList->GetSelectedItem() == ListItemObject)
	{
		SelectionImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		KY_LOG(LogKY, Log, TEXT("Selected : %s"), *ListItemObject->GetName());
	}
	else
	{
		SelectionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UKYItemSlotWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if(bIsEmpty) return;
	SelectionImage->SetVisibility(bIsSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UKYItemSlotWidget::ClearSlot()
{
	if (ItemIcon)
	{
		ItemIcon->SetBrush(FSlateBrush());
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ItemCount)
	{
		ItemCount->SetText(FText::AsNumber(0));
		ItemCount->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (SelectionImage)
	{
		SelectionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

FSlateBrush UKYItemSlotWidget::GetItemIconBrush(const TSharedPtr<FKYItemData>& InData)
{
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Type::NoDrawType;
	Brush.ImageSize = FVector2D(128.0f, 128.0f);
	if (InData)
	{
		Brush.DrawAs = ESlateBrushDrawType::Type::Image;
		Brush.SetResourceObject(InData->Icon);
	}
	
	return Brush;
}
