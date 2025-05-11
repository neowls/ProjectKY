// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "ProjectKY.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TileView.h"
#include "Components/VerticalBox.h"
#include "Data/KYInventoryItemObject.h"
#include "Player/KYPlayerState.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Player/KYInventoryViewModel.h"
#include "System/KYCharacterPreviewSubsystem.h"
#include "UI/Window/KYConfirmDialogWidget.h"

UKYInventoryTabWidget::UKYInventoryTabWidget()
	: SkeletalMeshAsset(nullptr), PreviewMaterial(nullptr), ViewModel(nullptr), LastSelectedViewIndex(0), SelectedSlotIndex(0), EquipSlotSelectMode(false), CurrentFocusViewState(EInventoryFocusViewState::None)
{}

void UKYInventoryTabWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// PlayerState 가져오기
	if (auto PlayerState = GetOwningPlayer()->GetPlayerState<AKYPlayerState>())
	{
		ViewModel = NewObject<UKYInventoryViewModel>(this);
		ViewModel->Initialize(PlayerState);
		ViewModel->OnItemSlotChanged.BindDynamic(this, &ThisClass::RefreshInventory);
		
		InitializeViews();
	}

	if (ConfirmDialog)
	{
		ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
		ConfirmDialog->OnActionSelected.AddUObject(this, &UKYInventoryTabWidget::OnDialogActionSelected);
	}

	SetupListView(InventoryGrid);
	SetupListView(WeaponList);
	SetupListView(ArmorList);
	
	SetupPreviewCharacter();
	ClearDetailPanel();
}

void UKYInventoryTabWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UKYCharacterPreviewSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UKYCharacterPreviewSubsystem>())
	{
		Subsystem->UpdateScene(InDeltaTime); // 프리뷰 캐릭터 메쉬와 렌더 타겟 텍스쳐를 업데이트 한다.
	}
}

// 뷰 세팅
void UKYInventoryTabWidget::InitializeViews()
{
	KY_LOG(LogKY, Log, TEXT("Initialize Views"));
	
	SetCategory(EKYItemType::None);
	RefreshEquipments();
}

void UKYInventoryTabWidget::RefreshInventory()
{
	InventoryGrid->SetListItems(ViewModel->GetVisibleInventory());
}

void UKYInventoryTabWidget::RefreshEquipments() const
{
	WeaponList->SetListItems(ViewModel->GetVisibleEquippedWeapons());
	ArmorList->SetListItems(ViewModel->GetVisibleEquippedArmors());
}


void UKYInventoryTabWidget::SetCategory(EKYItemType NewCategory)
{
	int32 Index = static_cast<int32>(NewCategory);

	for (auto Item : CategoryBox->GetAllChildren())
	{
		if (CategoryBox->GetChildAt(Index) == Item)
		{
			Item->SetRenderScale({1.2f, 1.2f});
			Item->SetRenderOpacity(1.0f);
		}
		else
		{
			Item->SetRenderScale({1.0f, 1.0f});
			Item->SetRenderOpacity(0.5f);
		}
	}

	ViewModel->SetFilterCategory(NewCategory);
	
	RefreshInventory();
}

// 아이템 선택 다이얼로그 창 표시
void UKYInventoryTabWidget::ShowItemDialog(const FKYItemData& ItemData)
{
	if (!ConfirmDialog) return;
	if (ItemData.IsEmpty()) return;
	
	ConfirmDialog->SetupDialog(ItemData);
	ConfirmDialog->SetVisibility(ESlateVisibility::Visible);
	ConfirmDialog->SetFocus();
	SelectedInstanceID = ItemData.InstanceData.InstanceID;
}

// 아이템 더블 클릭시 
void UKYInventoryTabWidget::OnHandleItemDoubleClicked(UObject* DoubleClickedItem)
{
	if (auto Wrapper = Cast<UKYInventoryItemObject>(DoubleClickedItem))
	{
		if (!Wrapper->IsEmpty())
		{
			ShowItemDialog(Wrapper->GetItemData());
		}
	}
}

void UKYInventoryTabWidget::OnHandleItemSelectionChanged(UObject* ChangedItem)
{
	auto Wrapper = Cast<UKYInventoryItemObject>(ChangedItem);
	if (!Wrapper) return;

	if (SelectedSlot == Wrapper) return;

	UListView* NewListView = Wrapper->GetItemData().InstanceData.EquipState == EKYEquipmentState::Inventory ? InventoryGrid : Wrapper->GetItemData().ItemType == EKYItemType::Armor ? ArmorList : WeaponList;
	if (LastSelectedView && LastSelectedView != NewListView) LastSelectedView->ClearSelection();
	LastSelectedView = NewListView;
	
	
	SelectedSlot = Wrapper;
	
	if (!Wrapper->IsEmpty())
	{
		UpdateDetailPanel(Wrapper->GetItemData());
		SelectedInstanceID = Wrapper->GetItemData().InstanceData.InstanceID;
	}
	else
	{
		ClearDetailPanel();
		SelectedInstanceID = NAME_None;
	}
}


// 델리게이트 설정
void UKYInventoryTabWidget::SetupListView(UListView* InListView)
{
	if (!InListView) return;

	InListView->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnHandleItemSelectionChanged);
	InListView->OnItemDoubleClicked().AddUObject(this, &ThisClass::OnHandleItemDoubleClicked);
	
}

// 다이얼로그 액션 선택
void UKYInventoryTabWidget::OnDialogActionSelected(FName Action)
{
	if (SelectedSlot->IsEmpty()) return;

	const auto& ItemData = SelectedSlot->GetItemData();
	if (ItemData.IsEmpty()) return;

	
	if (Action == "Use")
	{
		ViewModel->UseItem(ItemData.InstanceData.InstanceID);
	}
	else if (Action == "Drop")
	{
		ViewModel->DropItem(ItemData.InstanceData.InstanceID, 1);
	}
	else if (Action == "Equip")
	{
		if (ItemData.ItemType == EKYItemType::Weapon)
		{
			SelectWeaponSlot();
		}
		else
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			ViewModel->EquipItem(ItemData.InstanceData.InstanceID);
		}
	}
	else if (Action == "Unequip")
	{
		ViewModel->UnequipItem(ItemData.InstanceData.InstanceID, 0,SelectedSlot->GetItemData().ItemType);
	}
}

// 세부 정보 업데이트
void UKYInventoryTabWidget::UpdateDetailPanel(const FKYItemData& ItemData) const
{
	if (ItemData.IsEmpty())
	{
		ClearDetailPanel();
		return;
	}

	ItemNameText->SetText(ItemData.Name);
	ItemDescriptionText->SetText(ItemData.Description);
}

// 세부 정보 비우기 
void UKYInventoryTabWidget::ClearDetailPanel() const
{
	ItemNameText->SetText(FText::GetEmpty());
	ItemDescriptionText->SetText(FText::GetEmpty());
}

// 프리뷰 캐릭터 모델링 세팅
void UKYInventoryTabWidget::SetupPreviewCharacter()
{
	auto Subsystem = GetGameInstance()->GetSubsystem<UKYCharacterPreviewSubsystem>();
	if (Subsystem)
	{
		PreviewMaterialInstance = UMaterialInstanceDynamic::Create(PreviewMaterial, this); // 렌더 타겟을 실시간으로 받아야 하기에 다이나믹 머터리얼로 설정하였다.
		Subsystem->SetPreviewMesh(SkeletalMeshAsset, PreviewAnimSequence);
		UTextureRenderTarget2D* RT = Subsystem->GetPreviewRenderTarget();
		PreviewMaterialInstance->SetTextureParameterValue("PreviewTexture", RT);
		if (RT && CharacterModelImage)
		{
			// 이미지 위젯에 렌더 타겟 머터리얼 설정
			FSlateBrush Brush;
			Brush.SetImageSize(FVector2D(RT->SizeX,RT->SizeY));
			CharacterModelImage->SetBrush(Brush);
			CharacterModelImage->SetBrushFromMaterial(PreviewMaterialInstance);
		}
	}
}

void UKYInventoryTabWidget::UpdateCharacterPreview(FName InstanceID)
{
	if (InstanceID.IsNone()) return;
	
	// 장비 메시 로드 및 적용 로직 추가
	// 실제 구현은 프로젝트 상황에 따라 다를 수 있음
}


// 무기 장착시 슬롯 선택
void UKYInventoryTabWidget::SelectWeaponSlot()
{
	if (!WeaponList) return;
	
	EquipSlotSelectMode = true;
	LastSelectedView = WeaponList;

	// 빈슬롯 먼저 선택
	for (auto Wrapper : ViewModel->GetVisibleEquippedWeapons())
	{
		if (Wrapper->IsEmpty())
		{
			WeaponList->SetSelectedItem(Wrapper);
			SelectedSlotIndex = LastSelectedView->GetIndexForItem(Wrapper);
			return;
		}
	}

	// 없다면 첫번째 선택
	SelectedSlotIndex = 0;
	WeaponList->SetSelectedItem(WeaponList->GetItemAt(SelectedSlotIndex));
}

bool UKYInventoryTabWidget::NavigateView(const int32& DirX)
{
	if (LastSelectedView) return false;
	int32 NewIndex = FMath::Clamp(LastSelectedViewIndex + DirX, 1, 3);
	
	CurrentFocusViewState = static_cast<EInventoryFocusViewState>(NewIndex);
	LastSelectedViewIndex = NewIndex;
	
	switch (CurrentFocusViewState)
	{
	case EInventoryFocusViewState::None:
		return false;
	case EInventoryFocusViewState::Inventory:
		LastSelectedView = InventoryGrid;
		return true;
	case EInventoryFocusViewState::Weapon:
		LastSelectedView = WeaponList;
		return true;
	case EInventoryFocusViewState::Armor:
		LastSelectedView = ArmorList;
		return true;
	}
	
	return false;
}

bool UKYInventoryTabWidget::NavigateCategory(const int32& DirX, const int32& DirY)
{
	if (DirX != 0)
	{
		int32 NewIndex = static_cast<int32>(ViewModel->GetCurrentCategory()) + DirX;
		NewIndex = FMath::Clamp(NewIndex, 0, 4);
		SetCategory(static_cast<EKYItemType>(NewIndex));
		return true;
	}
	return false;
}



// 인벤토리 네비게이션
bool UKYInventoryTabWidget::NavigateInventory(const int32& DirX, const int32& DirY)
{
	if (!InventoryGrid) return false;

	uint8 GridColumns = 5;
	int32 Row = SelectedSlotIndex / GridColumns;
	int32 Col = SelectedSlotIndex % GridColumns;

	int32 NewRow = Row + DirY;
	int32 NewCol = Col + DirX;

	// 최상단을 벗어나면 카테고리로
	if (NewRow < 0)
	{
		LastSelectedView = nullptr;
		return true;
	}

	int32 NewIndex = FMath::Clamp(NewRow * GridColumns + NewCol, 0, InventoryGrid->GetNumItems() - 1);
	if (NewIndex == SelectedSlotIndex) return false;
	
	if (!InventoryGrid->GetItemAt(NewIndex)) return false;

	InventoryGrid->SetSelectedItem(InventoryGrid->GetItemAt(NewIndex));
	SelectedSlotIndex = NewIndex;
	return true;
}

bool UKYInventoryTabWidget::NavigateEquipment(const int32& DirY)
{
	UListView* TargetView = LastSelectedView == WeaponList ? WeaponList : ArmorList;
	if (!TargetView) return false;

	const int32 SlotIndex = TargetView->GetIndexForItem(TargetView->GetSelectedItem());
	const int32 NewSlotIndex = FMath::Clamp(SlotIndex + DirY, 0, TargetView->GetNumItems() - 1);
	if (SlotIndex == NewSlotIndex) return false;

	auto NewSlot = TargetView->GetItemAt(NewSlotIndex);
	TargetView->SetSelectedItem(NewSlot);
	SelectedSlotIndex = NewSlotIndex;
	
	return true;
}

void UKYInventoryTabWidget::SelectInventoryView()
{
	if (SelectedSlot) LastSelectedView->ClearSelection();
	
	LastSelectedView = InventoryGrid;
	
	// 현재 인벤토리가 비어있지 않다면 첫번째 슬롯을 바로 선택한다.
	if (ViewModel->GetVisibleInventory().Num() > 0)
	{
		SelectedSlotIndex = 0;
		InventoryGrid->SetSelectedItem(InventoryGrid->GetItemAt(SelectedSlotIndex));
	}
	// 인벤토리가 비어있다면 뷰만 선택한다. ( 카테고리 선택 상태 )
	else
	{
		SelectedSlotIndex = -1;
	}
}

bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	// 네비게이션 로직
	const int32 DirX = FMath::RoundToInt(AxisX);
	const int32 DirY = FMath::RoundToInt(AxisY);

	switch (CurrentFocusViewState)
	{
		case EInventoryFocusViewState::None:
			return NavigateView(DirX);
		
		case EInventoryFocusViewState::Inventory:
			return NavigateInventory(DirX, DirY);
		
		case EInventoryFocusViewState::Weapon:
		case EInventoryFocusViewState::Armor:
			return NavigateEquipment(DirY);
		
		default:
			return false;
	}
}

bool UKYInventoryTabWidget::HandleConfirmInput_Implementation()
{
	if (!InventoryGrid) return false;

	// 카테고리에서 선택키 입력시 인벤토리 선택
	if (!LastSelectedView)
	{
		SelectInventoryView();
		return true;
	}

	// 무기 슬롯 선택 모드
	if (EquipSlotSelectMode && LastSelectedView == WeaponList)
	{
		if (SelectedSlot)
		{
			const auto& ItemData = SelectedSlot->GetItemData();
			if (!ItemData.IsEmpty())
			{
				const bool Result = ViewModel->EquipItem(ItemData.InstanceData.InstanceID, SelectedSlotIndex);
				EquipSlotSelectMode = false;
				LastSelectedView = InventoryGrid;
				ClearDetailPanel();
				return Result;
			}
		}
	}
	
	// 확인 입력 처리 - 현재 선택된 아이템 액션 실행
	if (LastSelectedView && LastSelectedView->GetSelectedItem())
	{
		auto Wrapper = Cast<UKYInventoryItemObject>(LastSelectedView->GetSelectedItem());
		if (Wrapper && !Wrapper->IsEmpty())
		{
			ShowItemDialog(Wrapper->GetItemData());
			return true;
		}
	}

	return false;
} 

bool UKYInventoryTabWidget::HandleCancelInput_Implementation()
{
	// 취소 입력 처리 - 선택 초기화
	if (ConfirmDialog && ConfirmDialog->IsVisible())
	{
		ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
		return true;
	}

	// 무기 슬롯 선택 상태일 때 인벤토리로
	if (EquipSlotSelectMode)
	{
		EquipSlotSelectMode = false;
		LastSelectedView = InventoryGrid;
		LastSelectedView->SetSelectedItem(SelectedSlot);
		return true;
	}

	// 카테고리 선택으로
	if (SelectedInstanceID != NAME_None)
	{
		ClearDetailPanel();
		InventoryGrid->ClearSelection();
		LastSelectedView = nullptr;
		SelectedSlotIndex = 0;
		SelectedInstanceID = NAME_None;
		SelectedSlot = nullptr;
		return true;
	}
	
	return false;
}

bool UKYInventoryTabWidget::HandleWindowInput_Implementation()
{
	ClearDetailPanel();
	if (LastSelectedView) LastSelectedView->ClearSelection();
	LastSelectedView = nullptr;
	SelectedSlotIndex = 0;
	SelectedInstanceID = NAME_None;
	SelectedSlot = nullptr;
	return true;
}


