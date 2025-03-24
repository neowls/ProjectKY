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
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	virtual bool HandleConfirmInput_Implementation() override;
	virtual bool HandleCancelInput_Implementation() override;
	
protected:
	// 캐릭터 프리뷰
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;
    
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TObjectPtr<USceneCaptureComponent2D> CaptureComponent;
    
	UPROPERTY(EditDefaultsOnly, Category = "Preview")
	TObjectPtr<USkeletalMeshComponent> PreviewMeshComponent;

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
	TSubclassOf<UKYItemSlotWidget> WeaponSlotWidgetClass;

	// 아이템 사용 확인 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UKYUserWidget> ConfirmDialogClass;
	
	UPROPERTY()
	TObjectPtr<AKYPlayerState> OwningPlayerState;
	
	
private:
	// 현재 포커스된 컨테이너 타입
	enum class EFocusContainer
	{
		Inventory,
		Equipment,
		Weapon
	};
    
	// 그리드 설정
	static constexpr int32 GridColumns = 4;
	static constexpr int32 GridRows = 6;
	
	// 인벤토리 슬롯 관리
	UPROPERTY()
	TArray<UKYItemSlotWidget*> InventorySlots;
	
	// 장비 슬롯 관리 - 장비 타입별로 슬롯 매핑
	UPROPERTY()
	TMap<EKYEquipmentType, UKYItemSlotWidget*> EquipmentSlots;
	
	// 무기 슬롯 관리 - 무기 슬롯 인덱스별로 슬롯 매핑
	UPROPERTY()
	TMap<int32, UKYItemSlotWidget*> WeaponSlots;
	
	// 선택된 아이템 인스턴스 ID
	TObjectPtr<UKYItemSlotWidget> SelectedSlot;

	FName SelectedInstanceID;

	// 현재 포커스된 컨테이너
	EFocusContainer CurrentFocusContainer;
	
	// 이벤트 핸들러
	UFUNCTION()
	void OnSlotClicked(UKYItemSlotWidget* ClickedSlot);
	
	UFUNCTION()
	void OnInventoryChanged(FName InstanceID, bool bAdded);
	
	UFUNCTION()
	void OnEquipmentChanged(EKYEquipmentType EquipmentType, bool bEquipped);
	
	UFUNCTION()
	void OnWeaponStateChanged(int32 SlotIndex, bool bInHand);
	
	// UI 업데이트
	void ClearDetailPanel();
	void UpdateDetailPanel(FName InstanceID);
	
	void SetupPreviewCharacter();
	void UpdateCharacterPreview(FName InstanceID);
	
	void UpdateInventory();
	void UpdateEquipmentSlots();
	void UpdateWeaponSlots();
	
	// 초기화
	void InitializeInventorySlots();
	void InitializeEquipmentSlots();
	void InitializeWeaponSlots();
	void BindPlayerStateEvents();
	
	// 아이템 액션
	void UseItem(FName InstanceID);
	void EquipItem(FName InstanceID);
	void UnequipItem(FName InstanceID);
	void ToggleWeapon(FName InstanceID);
	void ShowUseItemDialog(FName InstanceID);
};



