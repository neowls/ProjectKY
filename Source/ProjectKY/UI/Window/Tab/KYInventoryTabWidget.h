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
class UTextureRenderTarget2D;
class USkeletalMeshComponent;
class AKYPlayerState;

UCLASS()
class PROJECTKY_API UKYInventoryTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	UKYInventoryTabWidget();
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	virtual bool HandleConfirmInput_Implementation() override;
	virtual bool HandleCancelInput_Implementation() override;
	
protected:
	// 인벤토리 각 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> EquipmentBox;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> WeaponBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterModelImage;


	// 아이템 세부 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

	// 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYItemSlotWidget> ItemSlotWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYItemSlotWidget> EquipmentSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYItemSlotWidget> WeaponSlotWidgetClass;

	// 아이템 사용 확인 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYUserWidget> ConfirmDialogClass;
	
	UPROPERTY()
	TObjectPtr<AKYPlayerState> OwningPlayerState;

	// 그리드 설정
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 GridColumns = 10;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 GridRows = 5;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<class UAnimationAsset> PreviewAnimSequence;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UMaterial* PreviewMaterial;


private:
	
	// 인벤토리 슬롯 관리
	UPROPERTY()
	TArray<UKYItemSlotWidget*> InventorySlots;
	
	UPROPERTY()
	TMap<uint8, UKYItemSlotWidget*> EquipmentSlots;
	
	// 선택된 아이템 인스턴스 ID
	UPROPERTY()
	TObjectPtr<UKYItemSlotWidget> SelectedSlot;

	uint8 SelectedSlotIndex;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> PreviewMaterialInstance;
	
	FName SelectedInstanceID;
	
	
	// 이벤트 핸들러
	UFUNCTION()
	void OnSlotClicked(UKYItemSlotWidget* ClickedSlot);
	
	UFUNCTION()
	void OnInventoryChanged(const FName& InstanceID, bool bAdded);
	
	UFUNCTION()
	void OnEquipmentChanged(const FName& InstanceID, bool bEquipped);
	
	UFUNCTION()
	void OnWeaponStateChanged(uint8 SlotIndex, bool bInHand);
	
	// UI 업데이트
	void ClearDetailPanel();
	void UpdateDetailPanel(FName InstanceID);
	
	void SetupPreviewCharacter();
	void UpdateCharacterPreview(FName InstanceID);
	
	void UpdateInventory();
	void UpdateEquipmentSlots();

	void SortInventory();

	void UpdateSelectedSlot();
	
	// 초기화
	void InitializeInventorySlots();
	void InitializeEquipmentSlots();
	void BindPlayerStateEvents();
	
	// 아이템 액션
	void UseItem(FName InstanceID);
	void ShowUseItemDialog(FName InstanceID);
};



