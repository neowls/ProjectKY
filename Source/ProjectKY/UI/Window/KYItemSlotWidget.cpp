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


// 오브젝트가 지정될 때 호출된다. (리스트뷰에 할당 시)
void UKYItemSlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	auto* Wrapper = Cast<UKYInventoryItemObject>(ListItemObject);
	if (!Wrapper) return;
	
	if (Wrapper->OnChanged.IsBound())
	{
		Wrapper->OnChanged.Clear();
	}

	if (Wrapper->IsValidLowLevel())
	{
		Wrapper->OnChanged.BindDynamic(this, &ThisClass::UpdateSlot);
	}
	else
	{
		KY_LOG(LogKY, Warning, TEXT("Wrapper Invalid Low Level"));
		return;
	}
	
	if (Wrapper->IsEmpty())
	{
		ClearSlot();
		return;
	}

	// 선택 체크
	UListView* OwningList = Cast<UListView>(GetOwningListView());
	if (OwningList && OwningList->GetSelectedItem() == ListItemObject)
	{
		SelectionImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		SelectionImage->SetVisibility(ESlateVisibility::Hidden);
	}

	auto const ItemData =  Wrapper->GetItemData();
	UpdateSlot(ItemData);
}

void UKYItemSlotWidget::UpdateSlot(const FKYItemData& InData)
{
	ItemIcon->SetBrush(GetItemIconBrush(InData));
	ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (InData.InstanceData.Count > 1)
	{
		ItemCount->SetText(FText::AsNumber(InData.InstanceData.Count));
		ItemCount->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}



void UKYItemSlotWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
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
		ItemCount->SetText(FText::GetEmpty());
		ItemCount->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (SelectionImage)
	{
		SelectionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

FSlateBrush UKYItemSlotWidget::GetItemIconBrush(const FKYItemData& InData)
{
	FSlateBrush Brush;
	Brush.DrawAs = ESlateBrushDrawType::Type::NoDrawType;
	Brush.ImageSize = FVector2D(128.0f, 128.0f);
	if (!InData.IsEmpty())
	{
		Brush.DrawAs = ESlateBrushDrawType::Type::Image;
		Brush.SetResourceObject(InData.Icon);
	}
	
	return Brush;
}
