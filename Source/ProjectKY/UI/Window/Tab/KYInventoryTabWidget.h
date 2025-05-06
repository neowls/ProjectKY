// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Interface/KYInputHandlerInterface.h"
#include "Struct/KYStruct.h"
#include "KYInventoryTabWidget.generated.h"

class UHorizontalBox;
class UKYInventoryViewModel;
class UKYConfirmDialogWidget;
class UKYInventoryItemObject;
class UListView;
class UTileView;
class UImage;
class UTextBlock;
class UTextureRenderTarget2D;
class USkeletalMeshComponent;


UCLASS()
class PROJECTKY_API UKYInventoryTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	UKYInventoryTabWidget();
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetCategory(EKYItemType NewCategory);

	// 전체 리스트 갱신
	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	void RefreshEquipments();

	// 이벤트 핸들러
	UFUNCTION()
	void OnHandleItemDoubleClicked(UObject* DoubleClickedItem);

	UFUNCTION()
	void OnHandleItemSelectionChanged(UObject* ChangedItem);

	UFUNCTION()
	void OnDialogActionSelected(FName Action);
	
protected:
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	virtual bool HandleConfirmInput_Implementation() override;
	virtual bool HandleCancelInput_Implementation() override;
	
	// 인벤토리 각 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTileView> InventoryGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> WeaponList;
	    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ArmorList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CategoryBox;
    
	
	// 캐릭터 모델 이미지
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterModelImage;

	// 아이템 사용 확인 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKYConfirmDialogWidget> ConfirmDialog;
	

	// 아이템 세부 정보
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<class UAnimationAsset> PreviewAnimSequence;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UMaterial* PreviewMaterial;

private:

	UPROPERTY()
	UKYInventoryViewModel* ViewModel;

	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleInventory;
	
	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleWeapons;
	
	UPROPERTY()
	TArray<TObjectPtr<UKYInventoryItemObject>> VisibleArmors;


	void HandleInventoryItemDataUpdated();

	void HandleInventoryItemSlotChanged();

	void HandleEquippedItemDataChanged();
	
	
	UPROPERTY()
	EKYItemType CurrentCategory;

	UPROPERTY()
	TObjectPtr<UKYInventoryItemObject> SelectedSlot;

	UPROPERTY()
	TObjectPtr<UListView> LastSelectedView;

	UPROPERTY()
	int32 SelectedSlotIndex;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> PreviewMaterialInstance;

	// 선택된 아이템 인스턴스 ID
	FName SelectedInstanceID;

	// 빈 데이터들을 담은 컨테이너
	UPROPERTY()
	TMap<FName ,TObjectPtr<UKYInventoryItemObject>> WrapperPool;


	void InitializeEquipmentSlots();
	
	// 세부 정보 갱신
	void UpdateDetailPanel(const TSharedPtr<FKYItemData>& ItemData);
	void ClearDetailPanel();
	
	void UpdateCharacterPreview(FName InstanceID);
	
	void SetupPreviewCharacter();

	void SelectFirstInventorySlot();

	bool NavigateInventory(int32 DirX, int32 DirY);
	bool NavigateEquipment(int32 DirY, bool bIsWeapon);
	
	// 아이템 액션
	void ShowItemDialog(const TSharedPtr<FKYItemData>& ItemData);
};



