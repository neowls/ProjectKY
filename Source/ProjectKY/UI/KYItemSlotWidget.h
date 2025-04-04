// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/KYStruct.h"
#include "UI/KYUserWidget.h"
#include "KYItemSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, class UKYItemSlotWidget*, Slot);

class UTextBlock;
class UBorder;
class UButton;
class UImage;

UCLASS()
class PROJECTKY_API UKYItemSlotWidget : public UKYUserWidget
{
	GENERATED_BODY()
	
public:
	// 슬롯 초기화
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeSlot();
    
	// 아이템 데이터로 슬롯 업데이트
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateSlot(const FKYInventoryWidgetData NewSlotData);
    
	// 슬롯 비우기
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearSlot();
	
	// 인스턴스 ID 접근자
	UFUNCTION(BlueprintPure, Category = "Inventory")
	FName GetInstanceID() const { return SlotData.InstanceID; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool GetIsSelected() const { return bIsSelected; }

	// 선택 상태 설정
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetIsSelected(bool bSelected);
    
	// 아이템 존재 여부
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem() const { return !SlotData.InstanceID.IsNone(); }

	// 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnSlotClicked OnSlotClicked;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FText GetItemCountText() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FSlateBrush GetItemIconBrush() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	ESlateVisibility GetItemCountVisibility() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	ESlateVisibility GetSelectionBorderVisibility() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	ESlateVisibility GetEquippedBorderVisibility() const;

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnSlotButtonClicked();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SelectionBorder;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> EquippedBorder;

private:
	// 인스턴스 ID
	UPROPERTY()
	FKYInventoryWidgetData SlotData;
	
	bool bIsSelected;
};
