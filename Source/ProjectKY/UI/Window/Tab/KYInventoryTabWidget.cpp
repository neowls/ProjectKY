// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "UI/KYItemSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item/KYItem.h"
#include "Player/KYPlayerState.h"

UKYInventoryTabWidget::UKYInventoryTabWidget()
{
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>("CaptureComponent2D");
	if (CaptureComponent)
	{
		CaptureComponent->FOVAngle = 45.0f;
		CaptureComponent->bCaptureEveryFrame = true;
		CaptureComponent->CaptureSource = SCS_SceneColorHDR;
	}

	PreviewMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("PreviewMeshComponent");
	if (PreviewMeshComponent)
	{
		PreviewMeshComponent->SetVisibility(true);
		PreviewMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//	Character Mesh Section
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(TEXT("/Game/NO_Cooking/Animation/AnimBaseCharacters/Mannequin_UE4/Meshes/SK_Mannequin_Base.SK_Mannequin_Base"));
		PreviewMeshComponent->SetSkeletalMeshAsset(SkeletalMeshRef.Object);
		static ConstructorHelpers::FObjectFinder<USkeleton> SkeletonRef(TEXT("/Game/NO_Cooking/Animation/AnimBaseCharacters/Mannequin_UE4/Meshes/SK_Mannequin_Skeleton_Base.SK_Mannequin_Skeleton_Base"));
		PreviewMeshComponent->GetSkeletalMeshAsset()->SetSkeleton(SkeletonRef.Object);
	}
}

void UKYInventoryTabWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// PlayerState 가져오기
	if (APlayerController* PC = GetOwningPlayer())
	{
		OwningPlayerState = Cast<AKYPlayerState>(PC->PlayerState);
	}

	SetupPreviewCharacter();
	
	InitializeInventorySlots();
	InitializeEquipmentSlots();
	InitializeWeaponSlots();

	BindPlayerStateEvents();

	UpdateInventory();
	UpdateEquipmentSlots();
	UpdateWeaponSlots();

	ClearDetailPanel();
}

void UKYInventoryTabWidget::InitializeInventorySlots()
{
	if (!InventoryGrid || !ItemSlotWidgetClass) return;
    
	// 인벤토리 그리드 슬롯 생성
	for (int32 i = 0; i < GridRows * GridColumns; ++i)
	{
		UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, ItemSlotWidgetClass);
		if (!NewSlot) continue;
        
		// 슬롯 초기화 - InstanceID는 아직 없으므로 NAME_None으로 설정
		NewSlot->InitializeSlot();
		NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
		int32 Row = i / GridColumns;
		int32 Column = i % GridColumns;
		InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
        
		InventorySlots.Add(NewSlot);
	}
}

void UKYInventoryTabWidget::InitializeEquipmentSlots()
{
	if (!EquipmentBox || !ItemSlotWidgetClass) return;
	
	TArray EquipmentTypes = {
		EKYEquipmentType::Head,
		EKYEquipmentType::Chest,
		EKYEquipmentType::Legs,
		EKYEquipmentType::Gloves,
		EKYEquipmentType::Feet
	};
	
	
	// 각 장비 슬롯 생성
	for (auto& Equipment : EquipmentTypes)
	{
		UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, ItemSlotWidgetClass);
		if (!NewSlot) continue;
        
		NewSlot->InitializeSlot();
		NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
		EquipmentBox->AddChild(NewSlot);
		EquipmentSlots.Add(Equipment, NewSlot);
	}
}

void UKYInventoryTabWidget::InitializeWeaponSlots()
{
	if (!WeaponBox || !WeaponSlotWidgetClass) return;
	
    
	for (int32 i = 0; i < 3; ++i)
	{
		UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, WeaponSlotWidgetClass);
		if (!NewSlot) continue;
        
		NewSlot->InitializeSlot();
		NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
		WeaponBox->AddChild(NewSlot);
		WeaponSlots.Add(i, NewSlot);
	}
}

void UKYInventoryTabWidget::BindPlayerStateEvents()
{
	if (!OwningPlayerState) return;
	
	OwningPlayerState->OnInventoryChanged.AddDynamic(this, &UKYInventoryTabWidget::OnInventoryChanged);
	OwningPlayerState->OnEquipmentChanged.AddDynamic(this, &UKYInventoryTabWidget::OnEquipmentChanged);
	OwningPlayerState->OnWeaponStateChanged.AddDynamic(this, &UKYInventoryTabWidget::OnWeaponStateChanged);
}

void UKYInventoryTabWidget::UpdateInventory()
{
	if (!OwningPlayerState) return;
	
	// 모든 인벤토리 슬롯 초기화
	for (UKYItemSlotWidget* ItemSlotWidget : InventorySlots)
	{
		ItemSlotWidget->ClearSlot();
	}
	
	// 인벤토리 아이템 목록 가져오기
	TArray<FName> InventoryItems = OwningPlayerState->GetInventoryItems();
	
	// 장착된 아이템 목록 제외 (장비슬롯에 표시될 것이므로)
	TArray<FName> EquippedItems = OwningPlayerState->GetEquippedItems();
	
	// 실제 인벤토리에 표시할 아이템들
	TArray<FName> ItemsToShow;
	
	for (const FName& ItemID : InventoryItems)
	{
		if (!EquippedItems.Contains(ItemID))
		{
			ItemsToShow.Add(ItemID);
		}
	}
	
	// 인벤토리 슬롯에 아이템 할당
	int32 SlotIndex = 0;
	for (const FName& InstanceID : ItemsToShow)
	{
		if (SlotIndex >= InventorySlots.Num()) break;
		
		const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
		if (!ItemData) continue;
		
		InventorySlots[SlotIndex]->UpdateSlot(InstanceID, *ItemData);
		
		// 현재 선택된 아이템이면 선택 상태 표시
		if (InstanceID == SelectedInstanceID)
		{
			InventorySlots[SlotIndex]->SetIsSelected(true);
		}
		
		SlotIndex++;
	}
}

void UKYInventoryTabWidget::UpdateEquipmentSlots()
{
	if (!OwningPlayerState) return;
	
	// 모든 장비 슬롯 초기화
	for (auto& Pair : EquipmentSlots)
	{
		Pair.Value->ClearSlot();
	}
	
	// 장착된 아이템 목록 가져오기
	TArray<FName> EquippedItems = OwningPlayerState->GetEquippedItems();
	
	for (const FName& InstanceID : EquippedItems)
	{
		const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
		if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) continue;
		
		// 장비 타입 확인
		UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
		if (!EquipItem) continue;
		
		// 무기가 아닌 일반 장비만 처리 (무기는 별도 처리)
		if (EquipItem->EquipmentType == EKYEquipmentType::Weapon) continue;
		
		// 해당 장비 슬롯에 아이템 표시
		if (UKYItemSlotWidget* NewSlot = EquipmentSlots.FindRef(EquipItem->EquipmentType))
		{
			NewSlot->UpdateSlot(InstanceID, *ItemData);
			
			// 현재 선택된 아이템이면 선택 상태 표시
			if (InstanceID == SelectedInstanceID)
			{
				NewSlot->SetIsSelected(true);
			}
		}
	}
}

void UKYInventoryTabWidget::UpdateWeaponSlots()
{
	if (!OwningPlayerState) return;
	
	// 모든 무기 슬롯 초기화
	for (auto& Pair : WeaponSlots)
	{
		Pair.Value->ClearSlot();
	}
	
	// 장착된 아이템 목록 가져오기
	TArray<FName> EquippedItems = OwningPlayerState->GetEquippedItems();
	
	for (const FName& InstanceID : EquippedItems)
	{
		const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
		if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) continue;
		
		// 장비 타입 확인
		UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
		if (!EquipItem || EquipItem->EquipmentType != EKYEquipmentType::Weapon) continue;
		
		// 무기 정보 확인
		UKYWeaponItem* WeaponItem = Cast<UKYWeaponItem>(EquipItem);
		if (!WeaponItem) continue;
		
		// 해당 무기 슬롯에 아이템 표시
		if (UKYItemSlotWidget* WeaponSlot = WeaponSlots.FindRef(WeaponItem->WeaponSlotIndex))
		{
			WeaponSlot->UpdateSlot(InstanceID, *ItemData);
			
			// 현재 선택된 아이템이면 선택 상태 표시
			if (InstanceID == SelectedInstanceID)
			{
				WeaponSlot->SetIsSelected(true);
			}
		}
	}
}

void UKYInventoryTabWidget::OnSlotClicked(UKYItemSlotWidget* ClickedSlot)
{
	if (!ClickedSlot) return;
	
	FName InstanceID = ClickedSlot->GetInstanceID();
    
	// 이전 선택 해제 (효율적으로!)
	if (SelectedSlot && SelectedSlot != ClickedSlot)
	{
		SelectedSlot->SetIsSelected(false);
	}
	
	// 새 선택 처리
	bool bIsSameItem = (SelectedSlot == ClickedSlot);
	SelectedInstanceID = InstanceID;
	SelectedSlot = ClickedSlot;
	SelectedSlot->SetIsSelected(true);
	
	// 세부 정보 업데이트
	UpdateDetailPanel(InstanceID);
	
	// 캐릭터 프리뷰 업데이트
	UpdateCharacterPreview(InstanceID);
	
	// 같은 아이템을 다시 클릭했다면 액션 실행
	if (bIsSameItem && OwningPlayerState)
	{
		const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
		if (!ItemData) return;
		
		// 아이템 타입에 따른 동작
		switch (ItemData->ItemType)
		{
			case EKYItemType::Equipment:
				{
					UKYEquipmentItem* EquipmentItem = Cast<UKYEquipmentItem>(ItemData->SubData);
					if (!EquipmentItem) return;
					
					// 장비 상태에 따른 동작
					if (EquipmentItem->bIsEquipped)
					{
						// 이미 장착 중이면 해제
						UnequipItem(InstanceID);
					}
					else
					{
						// 새로 장착
						EquipItem(InstanceID);
					}
				}
				break;
				
			case EKYItemType::Usable:
				// 소비 아이템 사용
				ShowUseItemDialog(InstanceID);
				break;
				
			default:
				break;
		}
	}
}

void UKYInventoryTabWidget::UseItem(FName InstanceID)
{
	if (!OwningPlayerState) return;
	
	if (OwningPlayerState->UseItem(InstanceID))
	{
		// 선택 초기화
		SelectedInstanceID = NAME_None;
		
		// UI 업데이트
		UpdateInventory();
		ClearDetailPanel();
	}
}

void UKYInventoryTabWidget::EquipItem(FName InstanceID)
{
	if (!OwningPlayerState) return;
	
	if (OwningPlayerState->EquipItem(InstanceID))
	{
		// UI 업데이트
		UpdateInventory();
		UpdateEquipmentSlots();
		UpdateWeaponSlots();
	}
}

void UKYInventoryTabWidget::UnequipItem(FName InstanceID)
{
	if (!OwningPlayerState) return;
	
	if (OwningPlayerState->UnequipItem(InstanceID))
	{
		// UI 업데이트
		UpdateInventory();
		UpdateEquipmentSlots();
		UpdateWeaponSlots();
	}
}

void UKYInventoryTabWidget::ToggleWeapon(FName InstanceID)
{
	if (!OwningPlayerState) return;
	
	if (OwningPlayerState->ToggleWeaponInHand(InstanceID))
	{
		// UI 업데이트 - 무기 슬롯만 업데이트
		UpdateWeaponSlots();
	}
}

void UKYInventoryTabWidget::ShowUseItemDialog(FName InstanceID)
{
	if (!OwningPlayerState || !ConfirmDialogClass) return;
	
	// 아이템 정보 확인
	const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
	if (!ItemData) return;
	
	// 다이얼로그 생성
	UKYUserWidget* Dialog = CreateWidget<UKYUserWidget>(this, ConfirmDialogClass);
	if (!Dialog) return;
	
	// 확인 콜백 설정 - 람다 함수 사용
	auto OnConfirm = [this, InstanceID]()
	{
		UseItem(InstanceID);
	};
	
	// 다이얼로그에 콜백 설정 및 화면에 추가
	// Dialog->SetConfirmCallback(OnConfirm);
	Dialog->AddToViewport();
}

void UKYInventoryTabWidget::UpdateDetailPanel(FName InstanceID)
{
	if (!OwningPlayerState || InstanceID.IsNone()) 
	{
		ClearDetailPanel();
		return;
	}
	
	const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
	if (!ItemData)
	{
		ClearDetailPanel();
		return;
	}
	
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemData->Name);
	}
	
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(ItemData->Description);
	}
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
	if (!CharacterModelImage || !RenderTarget || !CaptureComponent) return;
	
	// 렌더 타겟 설정
	CaptureComponent->TextureTarget = RenderTarget;
	
	// 이미지 위젯에 렌더 타겟 설정
	FSlateBrush Brush;
	Brush.SetResourceObject(RenderTarget);
	CharacterModelImage->SetBrush(Brush);
}

void UKYInventoryTabWidget::UpdateCharacterPreview(FName InstanceID)
{
	if (!OwningPlayerState || !PreviewMeshComponent || InstanceID.IsNone()) return;
	
	const FKYItemData* ItemData = OwningPlayerState->GetItemData(InstanceID);
	if (!ItemData || ItemData->ItemType != EKYItemType::Equipment) return;
	
	UKYEquipmentItem* EquipItem = Cast<UKYEquipmentItem>(ItemData->SubData);
	if (!EquipItem) return;
	
	// 장비 메시 로드 및 적용 로직 추가
	// 실제 구현은 프로젝트 상황에 따라 다를 수 있음
}

void UKYInventoryTabWidget::OnInventoryChanged(FName InstanceID, bool bAdded)
{
	UpdateInventory();
	
	// 변경된 아이템이 현재 선택된 아이템인 경우 처리
	if (InstanceID == SelectedInstanceID)
	{
		if (bAdded)
		{
			UpdateDetailPanel(InstanceID);
		}
		else
		{
			// 아이템이 제거된 경우 선택 초기화
			SelectedInstanceID = NAME_None;
			ClearDetailPanel();
		}
	}
}

void UKYInventoryTabWidget::OnEquipmentChanged(EKYEquipmentType EquipmentType, bool bEquipped)
{
	UpdateEquipmentSlots();
	UpdateInventory();
}

void UKYInventoryTabWidget::OnWeaponStateChanged(int32 SlotIndex, bool bInHand)
{
	UpdateWeaponSlots();
}

bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	// 네비게이션 로직 - InstanceID 기반으로 구현
	// 실제 구현은 프로젝트 요구사항에 따라 조정
	return false;
}

bool UKYInventoryTabWidget::HandleConfirmInput_Implementation()
{
	// 확인 입력 처리 - 현재 선택된 아이템 액션 실행
	if (SelectedInstanceID != NAME_None && OwningPlayerState)
	{
		const FKYItemData* ItemData = OwningPlayerState->GetItemData(SelectedInstanceID);
		if (!ItemData) return false;
		
		switch (ItemData->ItemType)
		{
			case EKYItemType::Equipment:
				{
					UKYEquipmentItem* EquipmentItem = Cast<UKYEquipmentItem>(ItemData->SubData);
					if (!EquipmentItem) return false;
					
					if (EquipmentItem->bIsEquipped)
					{
						UnequipItem(SelectedInstanceID);
					}
					else
					{
						EquipItem(SelectedInstanceID);
					}
				}
				return true;
				
			case EKYItemType::Usable:
				ShowUseItemDialog(SelectedInstanceID);
				return true;
				
			default:
				break;
		}
	}
	
	return false;
}

bool UKYInventoryTabWidget::HandleCancelInput_Implementation()
{
	// 취소 입력 처리 - 선택 초기화
	if (SelectedInstanceID != NAME_None)
	{
		// 모든 슬롯의 선택 상태 해제
		for (UKYItemSlotWidget* NewSlot : InventorySlots)
		{
			if (NewSlot->GetInstanceID() == SelectedInstanceID)
			{
				NewSlot->SetIsSelected(false);
			}
		}
		
		for (auto& Pair : EquipmentSlots)
		{
			if (Pair.Value->GetInstanceID() == SelectedInstanceID)
			{
				Pair.Value->SetIsSelected(false);
			}
		}
		
		for (auto& Pair : WeaponSlots)
		{
			if (Pair.Value->GetInstanceID() == SelectedInstanceID)
			{
				Pair.Value->SetIsSelected(false);
			}
		}
		
		SelectedInstanceID = NAME_None;
		ClearDetailPanel();
		return true;
	}
	
	return false;
}