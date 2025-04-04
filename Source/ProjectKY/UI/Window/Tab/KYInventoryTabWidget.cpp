// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYInventoryTabWidget.h"
#include "ProjectKY.h"
#include "UI/KYItemSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Player/KYPlayerState.h"
#include "Engine/TextureRenderTarget2D.h"
#include "System/KYCharacterPreviewSubsystem.h"

UKYInventoryTabWidget::UKYInventoryTabWidget()
{
	SelectedSlotIndex = 0;
	
	SkeletalMeshAsset = nullptr;
	PreviewMaterial = nullptr;
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

	BindPlayerStateEvents();

	UpdateInventory();
	UpdateEquipmentSlots();

	ClearDetailPanel();
}

void UKYInventoryTabWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (UKYCharacterPreviewSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UKYCharacterPreviewSubsystem>())
	{
		if (OwningPlayerState)	Subsystem->UpdateScene(InDeltaTime); // 프리뷰 캐릭터 메쉬와 렌더 타겟 텍스쳐를 업데이트 한다.
	}
}


void UKYInventoryTabWidget::InitializeInventorySlots()
{
	if (!InventoryGrid || !ItemSlotWidgetClass) return;
    
	// 인벤토리 그리드 슬롯 생성
	for (int32 i = 0; i < GridRows * GridColumns; ++i)
	{
		UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, ItemSlotWidgetClass);
		if (!NewSlot) continue;
		
		NewSlot->InitializeSlot();
		NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
		int32 Row = i / GridColumns;
		int32 Column = i % GridColumns;
		InventoryGrid->AddChildToUniformGrid(NewSlot, Row, Column);
        
		InventorySlots.Add(NewSlot);
	}
}

void UKYInventoryTabWidget::SortInventory()
{
	if (!OwningPlayerState) return;

	TArray AllItems = OwningPlayerState->GetInventoryWidgetArrayData();
	AllItems.Sort([] (const FKYInventoryWidgetData& A, const FKYInventoryWidgetData& B)
	{
		if (A.EquipState != B.EquipState)
   	 	{
   	 	    return A.EquipState > B.EquipState;
   	 	}
		
   	 	if (A.Type != B.Type)
   	 	{
   	 	    return A.Type > B.Type;
   	 	}
		
   	 	return A.Name.ToString() < B.Name.ToString();
	});
}

void UKYInventoryTabWidget::UpdateSelectedSlot()
{
	
}


void UKYInventoryTabWidget::InitializeEquipmentSlots()
{
	if (!EquipmentBox || !ItemSlotWidgetClass) return;
	
	uint8 StartIndex = static_cast<uint8>(EKYItemType::Head);
	uint8 SlotNum = 8;
	TArray<uint8> EquipmentIndex;
	EquipmentIndex.Reserve(SlotNum);

	for (uint8 i = 0; i < SlotNum; ++i)
	{
		EquipmentIndex.Add(StartIndex + i);
	}
	
	
	// 각 장비 슬롯 생성
	for (auto& Index : EquipmentIndex)
	{
		if (Index < static_cast<uint8>(EKYItemType::Weapon))
		{
			UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, EquipmentSlotWidgetClass);
			if (!NewSlot) continue;
        
			NewSlot->InitializeSlot();
			NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
			EquipmentBox->AddChild(NewSlot);
			EquipmentSlots.Add(Index, NewSlot);
		}
		else
		{
			UKYItemSlotWidget* NewSlot = CreateWidget<UKYItemSlotWidget>(this, WeaponSlotWidgetClass);
			if (!NewSlot) continue;
        
			NewSlot->InitializeSlot();
			NewSlot->OnSlotClicked.AddDynamic(this, &UKYInventoryTabWidget::OnSlotClicked);
        
			WeaponBox->AddChild(NewSlot);
			EquipmentSlots.Add(Index, NewSlot);
		}
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
	
	// 인벤토리 슬롯에 아이템 할당
	int32 SlotIndex = 0;
	
	for (const auto& Data : OwningPlayerState->GetInventoryWidgetArrayData())
	{
		if (SlotIndex >= InventorySlots.Num()) break;
		if(Data.Type == EKYItemType::None) continue;

		InventorySlots[SlotIndex]->UpdateSlot(Data);

		if (SelectedInstanceID == Data.InstanceID)
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
	for (auto& InSlot : EquipmentSlots)
	{
		InSlot.Value->ClearSlot();
	}

	// 장착된 아이템 목록만 업데이트 한다.
	for (const auto& InData : OwningPlayerState->GetEquippedItemsWidgetData())
	{
		if (EquipmentSlots.Contains(InData.Key))
		{
			EquipmentSlots[InData.Key]->UpdateSlot(InData.Value);
		}
	}
}

void UKYInventoryTabWidget::OnSlotClicked(UKYItemSlotWidget* ClickedSlot)
{
	if (!ClickedSlot) return;
	
	FName InstanceID = ClickedSlot->GetInstanceID();

	// 새 선택 처리
	bool bIsSameItem = (SelectedSlot == ClickedSlot);
	
	// 같지 않다면 이전 선택 해제
	if (!bIsSameItem)
	{
		SelectedSlot->SetIsSelected(false);
		SelectedSlot = ClickedSlot;
		SelectedInstanceID = InstanceID;
		SelectedSlot->SetIsSelected(true);

		// 세부 정보 업데이트
		UpdateDetailPanel(InstanceID);
	}
	// 같은 아이템을 다시 클릭했다면 액션 실행
	else if (bIsSameItem && OwningPlayerState)
	{
		FKYInventoryWidgetData ItemData = OwningPlayerState->GetInventoryWidgetData(InstanceID);
		
		// 아이템 타입에 따른 동작
		switch (ItemData.Type)
		{
			case EKYItemType::None:
				break;
			
			case EKYItemType::Misc:
				break;
			
			case EKYItemType::Usable:
				// 소비 아이템 사용
				ShowUseItemDialog(InstanceID);
				break;
			
			default:
				{
					// 장비 상태에 따른 동작
					if (ItemData.EquipState == EKYEquipmentState::Equipped)
					{
						// 이미 장착 중이면 해제
						OwningPlayerState->UnequipItem(InstanceID);
					}
					else
					{
						// 새로 장착
						OwningPlayerState->EquipItem(InstanceID);
					}
				}
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
	
	FKYInventoryWidgetData ItemData = OwningPlayerState->GetInventoryWidgetData(InstanceID);
	
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemData.Name);
	}
	
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(ItemData.Description);
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
	
	const FKYInventoryWidgetData ItemData = OwningPlayerState->GetInventoryWidgetData(InstanceID);
	if (ItemData.Type < EKYItemType::Head) return;

	
	
	// 장비 메시 로드 및 적용 로직 추가
	// 실제 구현은 프로젝트 상황에 따라 다를 수 있음
}

void UKYInventoryTabWidget::OnInventoryChanged(const FName& InstanceID, bool bAdded)
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

void UKYInventoryTabWidget::OnEquipmentChanged(const FName& InstanceID, bool bEquipped)
{
	//UpdateEquipmentSlots();
	// 캐릭터 프리뷰 업데이트
	UpdateCharacterPreview(InstanceID);
	// 해당 아이템의 Equipment 슬롯 및 인벤토리 슬롯 상태 변경.
}

void UKYInventoryTabWidget::OnWeaponStateChanged(uint8 SlotIndex, bool bInHand)
{
	//UpdateWeaponSlots();

	// 해당 아이템의 WeaponSlot 슬롯 및 인벤토리 슬롯 상태 변경.
}

bool UKYInventoryTabWidget::HandleNavigationInput_Implementation(float AxisX, float AxisY)
{
	// 네비게이션 로직
	SelectedSlotIndex = FMath::Clamp(SelectedSlotIndex + FMath::RoundToInt(AxisX) + FMath::RoundToInt(AxisY)*GridRows , 0, InventorySlots.Num()-1);
	return true;
}

bool UKYInventoryTabWidget::HandleConfirmInput_Implementation()
{
	
	// 확인 입력 처리 - 현재 선택된 아이템 액션 실행
	if (SelectedInstanceID != NAME_None && OwningPlayerState)
	{
		const FKYInventoryWidgetData ItemData = OwningPlayerState->GetInventoryWidgetData(SelectedInstanceID);
		if (ItemData.Type < EKYItemType::Head)
		{
			ShowUseItemDialog(SelectedInstanceID);
			return true;
		}
		
		if (ItemData.EquipState == EKYEquipmentState::Equipped)
		{
			OwningPlayerState->UnequipItem(SelectedInstanceID);
		}
		else
		{
			OwningPlayerState->EquipItem(SelectedInstanceID);
		}
		return true;
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
		
		/*
		for (auto& Pair : WeaponSlots)
		{
			if (Pair.Value->GetInstanceID() == SelectedInstanceID)
			{
				Pair.Value->SetIsSelected(false);
			}
		}
		*/
		
		SelectedInstanceID = NAME_None;
		ClearDetailPanel();
		return true;
	}
	
	return false;
}