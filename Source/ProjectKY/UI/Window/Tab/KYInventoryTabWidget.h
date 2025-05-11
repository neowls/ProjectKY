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

UENUM(BlueprintType)
enum class EInventoryFocusViewState : uint8
{
	None UMETA(DisplayName = "None"),
	Inventory UMETA(DisplayName = "Inventory"),
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor")
};


UCLASS()
class PROJECTKY_API UKYInventoryTabWidget : public UKYUserWidget, public IKYInputHandlerInterface
{
	GENERATED_BODY()

public:
	UKYInventoryTabWidget();
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(blueprintcallable, Category = "Inventory")
	void SetCategory(EKYItemType NewCategory);

	// 전체 리스트 갱신
	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	void RefreshEquipments() const;

	// 이벤트 핸들러
	UFUNCTION()
	void OnHandleItemDoubleClicked(UObject* DoubleClickedItem);

	UFUNCTION()
	void OnHandleItemSelectionChanged(UObject* ChangedItem);

	UFUNCTION()
	void OnDialogActionSelected(FName Action);

	void SetSlotSelectMode(const bool bInSlotSelectMode) { EquipSlotSelectMode = bInSlotSelectMode; }
	
protected:
	virtual bool HandleNavigationInput_Implementation(float AxisX, float AxisY) override;
	virtual bool HandleConfirmInput_Implementation() override;
	virtual bool HandleCancelInput_Implementation() override;
	virtual bool HandleWindowInput_Implementation() override;
	
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
	TObjectPtr<class UMaterialInstanceDynamic> PreviewMaterialInstance;
	
	UPROPERTY()
	UKYInventoryViewModel* ViewModel;

	UPROPERTY()
	TObjectPtr<UKYInventoryItemObject> SelectedSlot;

	UPROPERTY()
	TObjectPtr<UListView> LastSelectedView;

	uint8 LastSelectedViewIndex;

	UPROPERTY()
	int32 SelectedSlotIndex;

	UPROPERTY()
	FName SelectedInstanceID;

	UPROPERTY()
	bool EquipSlotSelectMode;

	EInventoryFocusViewState CurrentFocusViewState;
	
	void InitializeViews();
	
	// 세부 정보 갱신
	void UpdateDetailPanel(const FKYItemData& ItemData) const;
	void ClearDetailPanel() const;
	
	void UpdateCharacterPreview(FName InstanceID);
	
	void SetupPreviewCharacter();
	void SetupListView(UListView* InListView);
	
	void SelectWeaponSlot();
	
	bool NavigateView(const int32& DirX = 0);
	bool NavigateInventory(const int32& DirX = 0, const int32& DirY = 0);
	bool NavigateEquipment(const int32& DirY = 0);
	bool NavigateCategory(const int32& DirX = 0, const int32& DirY = 0);

	
	void SelectInventoryView();

	// 아이템 액션
	void ShowItemDialog(const FKYItemData& ItemData);
};



