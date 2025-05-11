// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Struct/KYStruct.h"
#include "KYItemSlotWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class PROJECTKY_API UKYItemSlotWidget : public UKYUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	
	
	// 슬롯 비우기
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearSlot();

	FSlateBrush GetItemIconBrush(const FKYItemData& InData);


protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateSlot(const FKYItemData& InData);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectionImage;

};
