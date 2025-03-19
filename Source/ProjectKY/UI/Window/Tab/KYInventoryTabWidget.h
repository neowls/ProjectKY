// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Interface/KYInputHandlerInterface.h"
#include "Struct/KYStruct.h"
#include "KYInventoryTabWidget.generated.h"

class UImage;
class UTextBlock;
class UVerticalBox;
class UUniformGridPanel;
class UKYItemSlotWidget;


UCLASS()
class PROJECTKY_API UKYInventoryTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

   
protected:
	// UI 컴포넌트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> EquipmentBox;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> WeaponBox;
    
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYItemSlotWidget> SlotWidgetClass;

	// 아이템 세부 정보 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> DetailItemIcon;

	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
    
private:
	// 슬롯 관리
	UPROPERTY()
	TMap<EKYSlotType, UKYItemSlotWidget*> AllSlots;
    
	// 선택된 슬롯
	UPROPERTY()
	TObjectPtr<UKYItemSlotWidget> SelectedSlot;
    
	// 이벤트 핸들러
	UFUNCTION()
	void OnSlotClicked(UKYItemSlotWidget* InSlot);
	
	// UI 업데이트
	void UpdateDetailPanel(const FKYItemData& ItemData);
	void ClearDetailPanel();
    
    
	// 그리드 설정
	static constexpr int32 GridColumns = 3;
	static constexpr int32 GridRows = 6;
};
