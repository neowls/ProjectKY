// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/KYUserWidget.h"
#include "Interface/KYInputHandlerInterface.h"
#include "Struct/KYStruct.h"
#include "KYInventoryTabWidget.generated.h"

class UKYConfirmDialogWidget;
class UKYInventoryItemObject;
class UListView;
class UTileView;
class UImage;
class UTextBlock;
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

	void SetCategory(EKYItemType NewCategory);
	
protected:
	// 인벤토리 각 패널
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTileView> InventoryGrid;
	    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ArmorList;
    
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> WeaponList;


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


	UPROPERTY()
	TObjectPtr<AKYPlayerState> OwningPlayerState;

	
	UPROPERTY()
	EKYItemType CurrentCategory;

	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<class UAnimationAsset> PreviewAnimSequence;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	USkeletalMesh* SkeletalMeshAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UMaterial* PreviewMaterial;


private:

	UPROPERTY()
	TObjectPtr<UKYInventoryItemObject> SelectedSlot;

	uint8 SelectedSlotIndex;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> PreviewMaterialInstance;

	// 선택된 아이템 인스턴스 ID
	FName SelectedInstanceID;

	// 빈 데이터들을 담은 컨테이너
	UPROPERTY()
	TSet<TObjectPtr<UKYInventoryItemObject>> WrapperPool;

	// 현재 UI에 보여지는 데이터들을 담은 컨테이너
	UPROPERTY()
	TSet<TObjectPtr<UKYInventoryItemObject>> InventoryWrappers;

	UPROPERTY()
	TMap<FName, TObjectPtr<UKYInventoryItemObject>> InventoryWrapperMap;

	UPROPERTY()
	TMap<uint8, TObjectPtr<UKYInventoryItemObject>> WeaponWrapperMap;

	UPROPERTY()
	TMap<EKYArmorSubType, TObjectPtr<UKYInventoryItemObject>> ArmorWrapperMap;

	
	
	// 이벤트 핸들러
	UFUNCTION()
	void OnHandleItemClicked(UObject* ClickedItem, UListView* SourceView);

	UFUNCTION()
	void OnDialogActionSelected(FName Action);
	
	// 세부 정보 갱신
	void UpdateDetailPanel(const TSharedPtr<FKYItemData>& ItemData);
	void ClearDetailPanel();
	
	void UpdateCharacterPreview(FName InstanceID);
	void SetupPreviewCharacter();

	
	void InitializeInventory();

	void InitializeEquipments();
	
	UKYInventoryItemObject* GetPooledWrapper();
	
	void RecycleWrapper(UKYInventoryItemObject* Wrapper);

	
	// 전체 리스트 갱신
	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	void RefreshEquipments();

	
	// 특정 슬롯 업데이트
	UFUNCTION()
	void UpdateInventoryItem(FName InstanceID);

	UFUNCTION()
	void UpdateArmorItem(EKYArmorSubType ArmorSlot);

	UFUNCTION()
	void UpdateWeaponItem(uint8 WeaponSlot);

	
	// 아이템 액션
	void ShowItemDialog(const TSharedPtr<FKYItemData>& ItemData);
};



