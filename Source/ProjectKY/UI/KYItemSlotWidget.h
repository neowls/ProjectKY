// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/KYStruct.h"
#include "UI/KYUserWidget.h"
#include "KYItemSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotEventSignature, class UKYItemSlotWidget*, Slot);

class UButton;
class UImage;

UCLASS()
class PROJECTKY_API UKYItemSlotWidget : public UKYUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetItemData(const FKYItemData& NewItemData, EKYSlotType InType, int32 InIndex = -1); 
	void ClearSlot();
	void SetIsSelected(bool NewSelect);
	
	UFUNCTION(BlueprintPure, Category = "Button")
	bool GetIsSelected() const { return bIsSelected; }

	UFUNCTION(BlueprintPure, Category = "Item")
	FKYItemData GetItemData() const { return ItemData; }

	int32 GetIndex() const { return SlotIndex; }

	bool IsEmpty() const { return ItemData.ItemID.IsNone(); }
	
	// 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnSlotEventSignature OnClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ItemButton;
    
private:
	UPROPERTY()
	FKYItemData ItemData;
    
	EKYSlotType SlotType;
	int32 SlotIndex;
	bool bIsSelected;
    
	UFUNCTION()
	void HandleButtonClicked();
    
	void UpdateVisuals();
};
