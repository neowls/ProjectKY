// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "ProjectKY.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/TileView.h"
#include "Data/KYInventoryItemObject.h"
#include "Player/KYPlayerState.h"
#include "Engine/TextureRenderTarget2D.h"
#include "System/KYCharacterPreviewSubsystem.h"
#include "UI/Window/KYConfirmDialogWidget.h"

UKYInventoryTabWidget::UKYInventoryTabWidget()
{
	SelectedSlotIndex = 0;
	SkeletalMeshAsset = nullptr;
	PreviewMaterial = nullptr;
	CurrentCategory = EKYItemType::None;
}

void UKYInventoryTabWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// PlayerState 가져오기
	if (APlayerController* PC = GetOwningPlayer())
	{
		OwningPlayerState = Cast<AKYPlayerState>(PC->PlayerState);
		OwningPlayerState->OnInventoryChanged.AddDynamic(this, &UKYInventoryTabWidget::UpdateInventoryItem);
	}

	// 람다 사용해서 Sender 바인딩
	if (InventoryGrid)
	{
		InventoryGrid->OnItemClicked().AddLambda([this](UObject* Item)
		{
			OnHandleItemClicked(Item, InventoryGrid);
		});
	}

	if (ArmorList)
	{
		ArmorList->OnItemClicked().AddLambda([this](UObject* Item)
		{
			OnHandleItemClicked(Item, ArmorList);
		});
	}

	if (WeaponList)
	{
		WeaponList->OnItemClicked().AddLambda([this](UObject* Item)
		{
			OnHandleItemClicked(Item, WeaponList);
		});
	}

	if (ConfirmDialog)
	{
		ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
		ConfirmDialog->OnActionSelected.AddUObject(this, &UKYInventoryTabWidget::OnDialogActionSelected);
	}

	SetupPreviewCharacter();
	ClearDetailPanel();

	InitializeInventory();
	InitializeEquipments();
	
	RefreshInventory();
}

void UKYInventoryTabWidget::InitializeInventory()
{
	if (!OwningPlayerState || !InventoryGrid) return;
	
	for (const auto& [Key, ItemData] : OwningPlayerState->GetInventoryMap())
	{
		if (ItemData.IsValid())
		{
			auto Wrapper = GetPooledWrapper();
			Wrapper->Initialize(ItemData);
			InventoryWrapperMap.Add(Key, Wrapper);
		}
	}
}

void UKYInventoryTabWidget::InitializeEquipments()
{
	if (!OwningPlayerState || !WeaponList || !ArmorList) return;

	// 고정 슬롯 생성
	for (auto Slot : OwningPlayerState->GetOrderedArmorSlots())
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->InitializeEmptyArmor(Slot);
		ArmorWrapperMap.Add(Slot, Wrapper);
		ArmorList->AddItem(Wrapper);
	}

	for (int i = 0; i < 3; i++)
	{
		auto Wrapper = NewObject<UKYInventoryItemObject>(this);
		Wrapper->InitializeEmptyWeapon(i);
		WeaponWrapperMap.Add(i, Wrapper);
		WeaponList->AddItem(Wrapper);
	}

	// 슬롯 업데이트
	for (const auto& [Key, ItemData]  : OwningPlayerState->GetEquippedArmorMap())
	{
		if (ItemData.IsValid())
		{
			if (auto Wrapper = ArmorWrapperMap.FindRef(Key))
			{
				Wrapper->SetData(ItemData);
			}
		}
	}

	for (const auto& [Key, ItemData]  : OwningPlayerState->GetEquippedWeaponMap())
	{
		if (ItemData.IsValid())
		{
			if (auto Wrapper = WeaponWrapperMap.FindRef(Key))
			{
				Wrapper->SetData(ItemData);
			}
		}
	}
	
}



void UKYInventoryTabWidget::SetCategory(EKYItemType NewCategory)
{
	CurrentCategory = NewCategory;
	RefreshInventory();
}

void UKYInventoryTabWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UKYCharacterPreviewSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UKYCharacterPreviewSubsystem>())
	{
		if (OwningPlayerState)	Subsystem->UpdateScene(InDeltaTime); // 프리뷰 캐릭터 메쉬와 렌더 타겟 텍스쳐를 업데이트 한다.
	}
}


void UKYInventoryTabWidget::RefreshInventory()
{
	if (!OwningPlayerState || !InventoryGrid) return;

	TArray<UObject*> WrapperList;

	for (auto& [Key, Wrapper] : InventoryWrapperMap)
	{
		if (!Wrapper) continue;
		if (TSharedPtr<FKYItemData> ItemData = Wrapper->GetItemPinned())
		{
			if (CurrentCategory == EKYItemType::None || CurrentCategory == ItemData->ItemType)
			{
				WrapperList.Add(Wrapper);
			}
		}
	}

	InventoryGrid->ClearListItems();
	InventoryGrid->SetListItems(WrapperList);
}

void UKYInventoryTabWidget::RefreshEquipments()
{
	if (!ArmorList || !WeaponList) return;

	TArray<UObject*> ArmorWrapperList;
	TArray<UObject*> WeaponWrapperList;
	
	// 슬롯 업데이트
	for (const auto& [Key, Wrapper]  : ArmorWrapperMap)
	{
		if (!Wrapper) continue;
		if (TSharedPtr<FKYItemData> ItemData = Wrapper->GetItemPinned())
		{
			ArmorWrapperList.Add(Wrapper);
		}
	}

	for (const auto& [Key, Wrapper]  : WeaponWrapperMap)
	{
		if (!Wrapper) continue;
		if (TSharedPtr<FKYItemData> ItemData = Wrapper->GetItemPinned())
		{
			ArmorWrapperList.Add(Wrapper);
		}
	}

	ArmorList->ClearListItems();
	ArmorList->SetListItems(ArmorWrapperList);

	WeaponList->ClearListItems();
	WeaponList->SetListItems(WeaponWrapperList);

	
}

void UKYInventoryTabWidget::UpdateInventoryItem(FName InstanceID)
{
	if (!InventoryWrapperMap.Contains(InstanceID) || !InventoryGrid) return;

	if (auto Wrapper = InventoryWrapperMap.FindRef(InstanceID))
	{
		TSharedPtr<FKYItemData> ItemData = Wrapper->GetItemPinned();
		if (!ItemData.IsValid()) return;

		const bool bVisible = InventoryGrid->IsItemVisible(Wrapper);
		const bool bShouldBeVisible = CurrentCategory == EKYItemType::None || CurrentCategory == ItemData->ItemType;

		if (bVisible && !bShouldBeVisible)
		{
			InventoryGrid->RemoveItem(Wrapper);
		}
		else if (!bVisible && bShouldBeVisible)
		{
			InventoryGrid->AddItem(Wrapper);
		}
	}
}

void UKYInventoryTabWidget::UpdateArmorItem(EKYArmorSubType ArmorSlot)
{
	if (!ArmorWrapperMap.Contains(ArmorSlot) || !ArmorList) return;

	
	
}

void UKYInventoryTabWidget::UpdateWeaponItem(uint8 WeaponSlot)
{
	if (!WeaponWrapperMap.Contains(WeaponSlot) || !WeaponList) return;

	
}


void UKYInventoryTabWidget::ShowItemDialog(const TSharedPtr<FKYItemData>& ItemData)
{
	if (!OwningPlayerState || !ConfirmDialog) return;
	if (!ItemData.IsValid()) return;
	
	ConfirmDialog->SetupDialog(ItemData);
	ConfirmDialog->SetVisibility(ESlateVisibility::Visible);
	ConfirmDialog->SetFocus();
	SelectedInstanceID = ItemData->InstanceData->GetFName();
}



UKYInventoryItemObject* UKYInventoryTabWidget::GetPooledWrapper()
{
	if (WrapperPool.Num() > 0)
	{
		auto It = WrapperPool.CreateIterator();
		auto Wrapper = *It;
		WrapperPool.Remove(Wrapper);
		return Wrapper;
	}
	
	auto NewWrapper = NewObject<UKYInventoryItemObject>(this);
	InventoryWrappers.Add(NewWrapper);
	return NewWrapper;
}

void UKYInventoryTabWidget::RecycleWrapper(UKYInventoryItemObject* Wrapper)
{
	if (!Wrapper) return;

	if (InventoryWrappers.Contains(Wrapper))
	{
		InventoryWrappers.Remove(Wrapper);
	}

	Wrapper->ClearData();
	WrapperPool.Add(Wrapper);
}

void UKYInventoryTabWidget::OnHandleItemClicked(UObject* ClickedItem, UListView* SourceView)
{
	if (!ClickedItem || !SourceView) return;
	auto* Wrapper = Cast<UKYInventoryItemObject>(ClickedItem);
	
	if (Wrapper && Wrapper->IsEmpty())
	{
		ClearDetailPanel();
		KY_LOG(LogKY, Warning, TEXT("Clear Data"));
		KY_LOG(LogKY, Warning, TEXT("Current Item : %s"), *Wrapper->GetName());
	}
	
	if (SourceView->GetSelectedItem() != ClickedItem)
	{
		TArray<UListView*> AllViews = { InventoryGrid, ArmorList, WeaponList };

		for (auto View : AllViews)
		{
			if (View && View != SourceView)
			{
				View->ClearSelection();
			}
		}
		if (Wrapper && !Wrapper->IsEmpty())
		{
			UpdateDetailPanel(Wrapper->GetItemPinned());
		}
	}
	else
	{
		ShowItemDialog(Cast<UKYInventoryItemObject>(ClickedItem)->GetItemPinned());
	}
	
	//SourceView->SetSelectedItem(ClickedItem);44
}


void UKYInventoryTabWidget::OnDialogActionSelected(FName Action)
{
	if (!OwningPlayerState) return;

	if (Action == "Cancel")
	{
		ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (Action == "Equip")
	{
		OwningPlayerState->EquipItem(SelectedInstanceID);
	}
	else if (Action == "Unequip")
	{
		OwningPlayerState->UnequipItem(SelectedInstanceID);
	}
	else if (Action == "Use")
	{
		OwningPlayerState->UseItem(SelectedInstanceID);
	}
	else if (Action == "Drop")
	{
		OwningPlayerState->RemoveItem(SelectedInstanceID);
	}
	
	RefreshInventory();
	ConfirmDialog->SetVisibility(ESlateVisibility::Collapsed);
}

void UKYInventoryTabWidget::UpdateDetailPanel(const TSharedPtr<FKYItemData>& ItemData)
{
	if (!ItemData.IsValid())
	{
		ClearDetailPanel();
		KY_LOG(LogKY, Warning, TEXT("Clear Data"));
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

void UKYInventoryTabWidget::UpdateCharacterPreview(FName InstanceID)
{
	if (!OwningPlayerState || InstanceID.IsNone()) return;
	
	
	// 장비 메시 로드 및 적용 로직 추가
	// 실제 구현은 프로젝트 상황에 따라 다를 수 있음
}


bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	// 네비게이션 로직
	//	SelectedSlotIndex = FMath::Clamp(SelectedSlotIndex + FMath::RoundToInt(AxisX) + FMath::RoundToInt(AxisY)*GridRows , 0, InventorySlots.Num()-1);
	return true;
}

bool UKYInventoryTabWidget::HandleConfirmInput_Implementation()
{
	if (!InventoryGrid) return false;
	
	// 확인 입력 처리 - 현재 선택된 아이템 액션 실행
	if (UObject* Selected = InventoryGrid->GetSelectedItem())
	{
		if (auto Wrapper = Cast<UKYInventoryItemObject>(Selected))
		{
			if (auto Data = Wrapper->GetItemPinned())
			{
				ShowItemDialog(Data);
				return true;
			}
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


