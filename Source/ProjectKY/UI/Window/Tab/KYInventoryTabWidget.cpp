// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "ProjectKY.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TileView.h"
#include "Data/KYInventoryItemObject.h"
#include "Player/KYPlayerState.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Player/KYInventoryViewModel.h"
#include "System/KYCharacterPreviewSubsystem.h"
#include "UI/Window/KYConfirmDialogWidget.h"

UKYInventoryTabWidget::UKYInventoryTabWidget()
{
	SelectedSlotIndex = 0;
	SkeletalMeshAsset = nullptr;
	PreviewMaterial = nullptr;
	ViewModel = nullptr;
	CurrentCategory = EKYItemType::None;
	
}

void UKYInventoryTabWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// PlayerState 가져오기
	if (auto PlayerState = GetOwningPlayer()->GetPlayerState<AKYPlayerState>())
	{
		ViewModel = NewObject<UKYInventoryViewModel>(this);
		ViewModel->Initialize(PlayerState);

		PlayerState->OnInventoryDataChanged.AddUObject(this, &ThisClass::HandleInventoryItemDataUpdated);
		PlayerState->OnInventorySlotChanged.AddUObject(this, &ThisClass::HandleInventoryItemSlotChanged);
		PlayerState->OnEquipmentChanged.AddUObject(this, &ThisClass::HandleEquippedItemDataChanged);

		RefreshInventory();
		InitializeEquipmentSlots();
	}

	if (ConfirmDialog)
	{
		ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
		ConfirmDialog->OnActionSelected.AddUObject(this, &UKYInventoryTabWidget::OnDialogActionSelected);
	}

	if (InventoryGrid)
	{
		InventoryGrid->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnHandleItemSelectionChanged);
		InventoryGrid->OnItemDoubleClicked().AddUObject(this, &ThisClass::OnHandleItemDoubleClicked);
	}

	if (WeaponList)
	{
		WeaponList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnHandleItemSelectionChanged);
		WeaponList->OnItemDoubleClicked().AddUObject(this, &ThisClass::OnHandleItemDoubleClicked);
	}

	if (ArmorList)
	{
		ArmorList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnHandleItemSelectionChanged);
		ArmorList->OnItemDoubleClicked().AddUObject(this, &ThisClass::OnHandleItemDoubleClicked);
	}
	
	SetupPreviewCharacter();
	ClearDetailPanel();
}



void UKYInventoryTabWidget::SetCategory(EKYItemType NewCategory)
{
	CurrentCategory = NewCategory;
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
	
	RefreshInventory();
}

void UKYInventoryTabWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UKYCharacterPreviewSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UKYCharacterPreviewSubsystem>())
	{
		Subsystem->UpdateScene(InDeltaTime); // 프리뷰 캐릭터 메쉬와 렌더 타겟 텍스쳐를 업데이트 한다.
	}
}

void UKYInventoryTabWidget::InitializeEquipmentSlots()
{
	VisibleWeapons.Reset();
	VisibleArmors.Reset();

	for (auto& Item : ViewModel->GetEquippedWeapons())
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->Initialize(Item);
		VisibleWeapons.Add(Wrapper);
	}

	for (auto& Item : ViewModel->GetEquippedArmors())
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->Initialize(Item);
		VisibleArmors.Add(Wrapper);
	}

	WeaponList->ClearListItems();
	WeaponList->SetListItems(VisibleWeapons);
	
	ArmorList->ClearListItems();
	ArmorList->SetListItems(VisibleArmors);
	
}



void UKYInventoryTabWidget::RefreshInventory()
{
	VisibleInventory.Reset();

	for (auto& Item : ViewModel->GetFilteredInventory(CurrentCategory))
	{
		const FName& ID = Item->InstanceData.InstanceID;
		UKYInventoryItemObject* Wrapper = WrapperPool.Contains(ID) ? WrapperPool.FindRef(ID) : *NewObject<UKYInventoryItemObject>(this);

		if (!WrapperPool.Contains(ID))
		{
			Wrapper->Initialize(Item);
			WrapperPool.Add(ID, Wrapper);
		}

		VisibleInventory.Add(Wrapper);
	}

	InventoryGrid->ClearListItems();
	InventoryGrid->SetListItems(VisibleInventory);
}

void UKYInventoryTabWidget::RefreshEquipments()
{
	VisibleWeapons.Reset();
	VisibleArmors.Reset();

	for (auto& Item : ViewModel->GetEquippedWeapons())
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->Initialize(Item);
		VisibleWeapons.Add(Wrapper);
	}

	for (auto& Item : ViewModel->GetEquippedArmors())
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->Initialize(Item);
		VisibleArmors.Add(Wrapper);
	}

	WeaponList->ClearListItems();
	WeaponList->SetListItems(VisibleWeapons);
	
	ArmorList->ClearListItems();
	ArmorList->SetListItems(VisibleArmors);
	
}

void UKYInventoryTabWidget::ShowItemDialog(const TSharedPtr<FKYItemData>& ItemData)
{
	if (!ConfirmDialog) return;
	if (!ItemData.IsValid()) return;
	if (ItemData->IsEmpty()) return;
	
	ConfirmDialog->SetupDialog(ItemData);
	ConfirmDialog->SetVisibility(ESlateVisibility::Visible);
	ConfirmDialog->SetFocus();
	SelectedInstanceID = ItemData->InstanceData.InstanceID;
}

void UKYInventoryTabWidget::OnHandleItemDoubleClicked(UObject* DoubleClickedItem)
{
	if (!DoubleClickedItem) return;
	auto Wrapper = Cast<UKYInventoryItemObject>(DoubleClickedItem);
	if (!Wrapper) return;
	
	if (Wrapper->GetItemPinned().IsValid() && !Wrapper->IsEmpty())
	{
		ShowItemDialog(Wrapper->GetItemPinned());
	}
}

void UKYInventoryTabWidget::OnHandleItemSelectionChanged(UObject* ChangedItem)
{
	if (!ChangedItem) return;
	auto Wrapper = Cast<UKYInventoryItemObject>(ChangedItem);
	if (!Wrapper) return;
	
	TArray<UListView*> ViewList = {InventoryGrid, ArmorList, WeaponList};
	UListView* CurrentView = nullptr;
	
	for (UListView* View :  ViewList)
	{
		if (View->GetSelectedItem() == ChangedItem)
		{
			CurrentView = View;
			break;
		}
	}

	if (LastSelectedView && LastSelectedView != CurrentView)
	{
		LastSelectedView->ClearSelection();
	}
	
	if (!Wrapper->IsEmpty())
	{
		UpdateDetailPanel(Wrapper->GetItemPinned());
		SelectedInstanceID = Wrapper->GetItemPinned()->InstanceData.InstanceID;
	}
	else
	{
		ClearDetailPanel();
		SelectedInstanceID = NAME_None;
	}

	LastSelectedView = CurrentView;
	SelectedSlot = Wrapper;
	
	KY_LOG(LogKY, Warning, TEXT("Slot Empty : %s"), Wrapper->IsEmpty() ? TEXT("True") : TEXT("False"));
	KY_LOG(LogKY, Warning, TEXT("Item Type : %d"), Wrapper->GetItemPinned()->ItemType);
	KY_LOG(LogKY, Warning, TEXT("Item Name : %s"), *Wrapper->GetItemPinned()->Name.ToString());
}


void UKYInventoryTabWidget::OnDialogActionSelected(FName Action)
{
	if (SelectedSlot->IsEmpty()) return;

	if (Action == "Use")
	{
		ViewModel->UseItem(SelectedSlot->GetItemPinned()->InstanceData.InstanceID);
	}
	else if (Action == "Drop")
	{
		ViewModel->DropItem(SelectedSlot->GetItemPinned()->InstanceData.InstanceID, 1);
	}
	else if (Action == "Equip")
	{
		ViewModel->EquipItem(SelectedSlot->GetItemPinned()->InstanceData.InstanceID, 0);
	}
	else if (Action == "Unequip")
	{
		//ViewModel->UnequipItem(SelectedSlot->GetItemPinned()->InstanceData->GetFName(), SelectedItem->InstanceData->AdditionalSlotIndex);
	}
}



void UKYInventoryTabWidget::HandleInventoryItemDataUpdated()
{
	InventoryGrid->RequestRefresh();
}

void UKYInventoryTabWidget::HandleInventoryItemSlotChanged()
{
	RefreshInventory();
}

void UKYInventoryTabWidget::HandleEquippedItemDataChanged()
{
	RefreshEquipments();
}


void UKYInventoryTabWidget::UpdateDetailPanel(const TSharedPtr<FKYItemData>& ItemData)
{
	if (!ItemData.IsValid())
	{
		ClearDetailPanel();
		return;
	}

	ItemNameText->SetText(ItemData->Name);
	ItemDescriptionText->SetText(ItemData->Description);
}

void UKYInventoryTabWidget::ClearDetailPanel()
{
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::GetEmpty());
	}
	
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(FText::GetEmpty());
	}
}

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

void UKYInventoryTabWidget::SelectFirstInventorySlot()
{
	for (int32 i = 0; i < InventoryGrid->GetListItems().Num(); ++i)
	{
		if (!VisibleInventory[i]->IsEmpty())
		{
			InventoryGrid->SetSelectedItem(VisibleInventory[0]);
			break;
		}
	}
}

void UKYInventoryTabWidget::UpdateCharacterPreview(FName InstanceID)
{
	if (InstanceID.IsNone()) return;
	
	
	// 장비 메시 로드 및 적용 로직 추가
	// 실제 구현은 프로젝트 상황에 따라 다를 수 있음
}


bool UKYInventoryTabWidget::NavigateInventory(int32 DirX, int32 DirY)
{
	uint8 GridColumns = 5;
	int32 Row = SelectedSlotIndex / GridColumns;
	int32 Col = SelectedSlotIndex % GridColumns;

	int32 NewRow = Row + DirY;
	int32 NewCol = Col + DirX;

	// 위로 올라가면 카테고리로
	if (NewRow < 0)
	{
		InventoryGrid->ClearSelection();
		LastSelectedView = nullptr;
		SelectedSlotIndex = 0;
		ClearDetailPanel();
		return true;
	}

	// 오른쪽 벗어나면 무기 슬롯
	if (NewCol >= GridColumns)
	{
		// Select First Weapon Slot
		WeaponList->SetSelectedItem(VisibleWeapons[0]);
		return true;
	}

	// 위치 계산
	int32 NewIndex = NewRow * GridColumns + NewCol;
	if (NewIndex == SelectedSlotIndex) return false;
	if (!VisibleInventory.IsValidIndex(NewIndex)) return false;
	if (VisibleInventory[NewIndex]->IsEmpty()) return false;
	
	InventoryGrid->SetSelectedItem(VisibleInventory[NewIndex]);
	SelectedSlotIndex = NewIndex;
	
	return true;
}

bool UKYInventoryTabWidget::NavigateEquipment(int32 DirY, bool bIsWeapon)
{
	TArray<TObjectPtr<UKYInventoryItemObject>>& TargetArray = bIsWeapon ? VisibleWeapons : VisibleArmors;
	UListView* View = bIsWeapon ? WeaponList : ArmorList;
	int32 SlotIndex = bIsWeapon ? WeaponList->GetIndexForItem(WeaponList->GetSelectedItem()) : ArmorList->GetIndexForItem(ArmorList->GetSelectedItem());

	int32 NewIndex = FMath::Clamp(SlotIndex + DirY, 0, TargetArray.Num() - 1);
	if (TargetArray.IsValidIndex(NewIndex) && !TargetArray[NewIndex]->IsEmpty())
	{
		OnHandleItemSelectionChanged(TargetArray[NewIndex]);
		return true;
	}
	return false;
}

bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	// 네비게이션 로직
	int32 DirX = FMath::RoundToInt(AxisX);
	int32 DirY = FMath::RoundToInt(AxisY);

	if (!LastSelectedView)
	{
		if (DirX != 0)
		{
			SetCategory(static_cast<EKYItemType>(FMath::Clamp(static_cast<int32>(CurrentCategory) + DirX, 0, 4)));
			return true;
		}

		if (DirY > 0)
		{
			SelectFirstInventorySlot();
			return true;
		}

		return false;
	}

	if (LastSelectedView == InventoryGrid)
	{
		return NavigateInventory(DirX, DirY);
	}

	if (LastSelectedView == WeaponList)
	{
		return NavigateEquipment(DirY, true);
	}

	if (LastSelectedView == ArmorList)
	{
		return NavigateEquipment(DirY, false);
	}

	return false;
}

bool UKYInventoryTabWidget::HandleConfirmInput_Implementation()
{
	if (!InventoryGrid) return false;
	
	// 확인 입력 처리 - 현재 선택된 아이템 액션 실행
	if (LastSelectedView && LastSelectedView->GetSelectedItem())
	{
		auto Wrapper = Cast<UKYInventoryItemObject>(LastSelectedView->GetSelectedItem());
		if (Wrapper && Wrapper->GetItemPinned().IsValid())
		{
			ShowItemDialog(Wrapper->GetItemPinned());
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
	
	if (SelectedInstanceID != NAME_None)
	{
		SelectedInstanceID = NAME_None;
		SelectedSlot = nullptr;
		ClearDetailPanel();
		return true;
	}
	
	return false;
}


