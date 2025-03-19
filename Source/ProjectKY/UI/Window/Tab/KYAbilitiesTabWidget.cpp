// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/Tab/KYAbilitiesTabWidget.h"

#include "ProjectKY.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/KYVideoWidget.h"
#include "UI/KYButtonWidget.h"
#include "Delegates/Delegate.h"
#include "Blueprint/WidgetTree.h"
#include "System/KYGameSingleton.h"

UKYAbilitiesTabWidget::UKYAbilitiesTabWidget()
{
    SelectedCategoryIndex = -1;
    SelectedAbilityIndex = -1;
    NavigationMode = 0; // 기본적으로 카테고리 선택 모드
    CurrentCategory = EKYAbilityCategory::Common;

    AllCategories =
    {
        EKYAbilityCategory::Common,
        EKYAbilityCategory::OneSword,
        EKYAbilityCategory::DualBlade,
        EKYAbilityCategory::GreatSword
    };
    
    for (EKYAbilityCategory Category : AllCategories)
    {
        CategoryAbilityMap.Add(Category, TArray<FKYAbilityData>());
    }
}

void UKYAbilitiesTabWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    CheckInitializedWidgets();

    // 게임 인스턴스에서 어빌리티 데이터 로드
    TArray<FKYAbilityData> AllAbilities;
    for (auto Row : UKYGameSingleton::Get().AbilityDataRows())
    {
        AllAbilities.Add(*Row);
    }
    
    SetAbilityData(AllAbilities);
}

void UKYAbilitiesTabWidget::SetAbilityData(const TArray<FKYAbilityData>& InAbilityDataArray)
{
    AbilityArray.Empty();

    for (const auto& Ability : InAbilityDataArray)
    {
        AbilityArray.Add(Ability);
        CategoryAbilityMap.Find(Ability.AbilityCategory)->Add(Ability);
    }
    
    InitializeCategoryList();
    UpdateAbilityList(CurrentCategory);
    ClearAbilityDetails();
}

// 바인딩 위젯 체크
void UKYAbilitiesTabWidget::CheckInitializedWidgets() const
{
    // ensure를 사용하여 각 위젯의 초기화 상태를 개별적으로 확인
    ensure(CategoryScrollBox && "CategoryScrollBox is not initialized");
    ensure(AbilityScrollBox && "AbilityScrollBox is not initialized");
    ensure(AbilityVideoWidget && "AbilityVideoWidget is not initialized");
    ensure(AbilityDescriptionText && "AbilityDescriptionText is not initialized");
    ensure(AbilityCommandText && "AbilityCommandText is not initialized");
    ensure(AbilityNameText && "AbilityNameText is not initialized");
}

void UKYAbilitiesTabWidget::ClearAbilityDetails()
{
    // 세부 정보 초기화
    AbilityNameText->SetText(FText::GetEmpty());
    AbilityDescriptionText->SetText(FText::GetEmpty());
    AbilityCommandText->SetText(FText::GetEmpty());
    AbilityVideoWidget->SetVisibility(ESlateVisibility::Hidden);
    AbilityVideoWidget->StopVideo();

    // 선택된 어빌리티 버튼이 있다면 초기화
    if (AbilityButtonWidgets.IsValidIndex(SelectedAbilityIndex))
    {
        AbilityButtonWidgets[SelectedAbilityIndex]->SetIsSelected(false);
        KY_LOG(LogKY, Log, TEXT("%s 선택 해제"), *AbilityButtonWidgets[SelectedAbilityIndex]->GetName());
        SelectedAbilityIndex = -1;
    }
}

void UKYAbilitiesTabWidget::InitializeCategoryList()
{
    // 기존 목록 초기화
    CategoryScrollBox->ClearChildren();
    CategoryButtonWidgets.Empty();
    
    // 버튼 위젯 클래스 확인
    if (!CategoryButtonWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Category Button Widget Class is not set"));
        return;
    }

    int32 CategoryButtonIndex = 0;
    // 카테고리 목록 생성
    for (auto& Category : CategoryAbilityMap)
    {
        FText NewCategoryName;
        
        switch (Category.Key)
        {
        case EKYAbilityCategory::Common:
            NewCategoryName = FText::FromString("Common");
            break;
        case EKYAbilityCategory::OneSword:
            NewCategoryName = FText::FromString("One Sword");
            break;
        case EKYAbilityCategory::DualBlade:
            NewCategoryName = FText::FromString("Dual Blade");
            break;
        case EKYAbilityCategory::GreatSword:
            NewCategoryName = FText::FromString("Great Sword");
            break;
        }
        
        // 버튼 위젯 생성
        UKYButtonWidget* ButtonWidget = CreateWidget<UKYButtonWidget>(this, CategoryButtonWidgetClass);
        if (ButtonWidget)
        {
            // 버튼 설정
            ButtonWidget->SetButtonIndex(CategoryButtonIndex);
            ButtonWidget->SetIsSelected(CategoryButtonIndex == SelectedAbilityIndex);
            ButtonWidget->DefaultButtonText = NewCategoryName;
            
            // 클릭 이벤트 바인딩
            ButtonWidget->OnButtonClickedWithIndex.AddDynamic(this, &UKYAbilitiesTabWidget::OnCategoryButtonClicked);
            
            // 스크롤 박스에 추가
            CategoryScrollBox->AddChild(ButtonWidget);
            
            // 배열에 추가
            CategoryButtonWidgets.Add(ButtonWidget);

            CategoryButtonIndex++;
        }
    }
}

void UKYAbilitiesTabWidget::UpdateAbilityList(const EKYAbilityCategory Category)
{
    if (!AbilityScrollBox) return;

    // 기존 목록 초기화
    AbilityScrollBox->ClearChildren();
    AbilityButtonWidgets.Empty();
    SelectedAbilityIndex = -1;

    // 버튼 위젯 클래스 확인
    if (!AbilityButtonWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability Button Widget Class is not set"));
        return;
    }
    
    // 선택된 카테고리의 어빌리티 목록 가져오기
    if (!CategoryAbilityMap.Contains(Category) || CategoryAbilityMap[Category].Num() == 0)
    {
        ClearAbilityDetails();
        return;
    }

    const TArray<FKYAbilityData>& Abilities = CategoryAbilityMap[Category];
    CreateAbilityButtons(Abilities);
}

void UKYAbilitiesTabWidget::CreateAbilityButtons(const TArray<FKYAbilityData>& Abilities)
{
    for (int32 AbilityButtonIndex = 0; AbilityButtonIndex < Abilities.Num(); AbilityButtonIndex++)
    {
        UKYButtonWidget* ButtonWidget = CreateWidget<UKYButtonWidget>(this, AbilityButtonWidgetClass);
        if (ButtonWidget)
        {
            ButtonWidget->SetButtonIndex(AbilityButtonIndex);
            ButtonWidget->SetIsSelected(false);
            ButtonWidget->DefaultButtonText = Abilities[AbilityButtonIndex].AbilityName;

            // 클릭 이벤트 바인딩
            ButtonWidget->OnButtonClickedWithIndex.AddDynamic(this, &UKYAbilitiesTabWidget::OnAbilityButtonClicked);

            // 스크롤 박스에 부착
            AbilityScrollBox->AddChild(ButtonWidget);
            
            // 버튼 배열에 추가
            AbilityButtonWidgets.Add(ButtonWidget);
        }
    }
}

void UKYAbilitiesTabWidget::ChangeSelectedCategory(const int32 NewIndex)
{
    // 범위 체크
    if (NewIndex < 0 || NewIndex >= CategoryButtonWidgets.Num())
    {
        return;
    }
    
    // 이전 선택 항목 비활성화
    if (SelectedCategoryIndex >= 0 && SelectedCategoryIndex < CategoryButtonWidgets.Num())
    {
        CategoryButtonWidgets[SelectedCategoryIndex]->SetIsSelected(false);
        ClearAbilityDetails();
    }
    
    // 새 선택 항목 활성화
    SelectedCategoryIndex = NewIndex;
    if (SelectedCategoryIndex >= 0 && SelectedCategoryIndex < CategoryButtonWidgets.Num())
    {
        CategoryButtonWidgets[SelectedCategoryIndex]->SetIsSelected(true);
        
        // 스크롤 박스에서 선택된 항목이 보이도록 스크롤
        if (CategoryScrollBox)
        {
            CategoryScrollBox->ScrollWidgetIntoView(CategoryButtonWidgets[SelectedCategoryIndex], false);
        }
    }
    
    // 선택된 카테고리에 따라 현재 카테고리 설정
    if (SelectedCategoryIndex >= 0 && SelectedCategoryIndex < AllCategories.Num())
    {
        CurrentCategory = AllCategories[SelectedCategoryIndex];
        AbilityArray = CategoryAbilityMap[CurrentCategory];
    }
    
    // 어빌리티 목록 업데이트
    UpdateAbilityList(CurrentCategory);
}

void UKYAbilitiesTabWidget::ChangeSelectedAbility(const int32 NewIndex)
{
    // 범위 체크
    if (NewIndex < 0 || NewIndex >= AbilityButtonWidgets.Num() || NewIndex == SelectedAbilityIndex)
    {
        return;
    }
    
    // 이전 선택 항목 비활성화
    if (SelectedAbilityIndex >= 0 && SelectedAbilityIndex < AbilityButtonWidgets.Num())
    {
        AbilityButtonWidgets[SelectedAbilityIndex]->SetIsSelected(false);
    }
    
    // 새 선택 항목 활성화
    SelectedAbilityIndex = NewIndex;
    if (SelectedAbilityIndex >= 0 && SelectedAbilityIndex < AbilityButtonWidgets.Num())
    {
        AbilityButtonWidgets[SelectedAbilityIndex]->SetIsSelected(true);
        
        // 스크롤 박스에서 선택된 항목이 보이도록 스크롤
        if (AbilityScrollBox)
        {
            AbilityScrollBox->ScrollWidgetIntoView(AbilityButtonWidgets[SelectedAbilityIndex], false);
        }
    }
    
    // 어빌리티 세부 정보 업데이트
    UpdateAbilityDetails();
}


void UKYAbilitiesTabWidget::UpdateAbilityDetails()
{
    // 선택된 카테고리의 어빌리티 목록 가져오기
    if (!CategoryAbilityMap.Contains(CurrentCategory))
    {
        ClearAbilityDetails();
        return;
    }
    
    const TArray<FKYAbilityData>& Abilities = CategoryAbilityMap[CurrentCategory];
    
    // 범위 체크
    if (SelectedAbilityIndex < 0 || SelectedAbilityIndex >= Abilities.Num())
    {
        // 세부 정보 초기화
        ClearAbilityDetails();
        return;
    }

    // 어빌리티 데이터 가져오기
    const FKYAbilityData& AbilityData = Abilities[SelectedAbilityIndex];

    // 비디오 설정
    if (AbilityVideoWidget)
    {
        if (AbilityData.AbilityPreviewMediaSource)
        {
            AbilityVideoWidget->SetVideo(AbilityData.AbilityPreviewMediaSource);
            AbilityVideoWidget->SetVisibility(ESlateVisibility::Visible);
            AbilityVideoWidget->PlayVideo();
        }
        else
        {
            AbilityVideoWidget->SetVisibility(ESlateVisibility::Hidden);
            AbilityVideoWidget->StopVideo();
        }
    }
    
    // 이름 설정
    if (AbilityNameText)
    {
        AbilityNameText->SetText(AbilityData.AbilityName);
    }
    
    // 설명 설정
    if (AbilityDescriptionText)
    {
        AbilityDescriptionText->SetText(AbilityData.AbilityDescription);
    }

    // 커맨드 설정
    if (AbilityCommandText)
    {
        AbilityCommandText->SetText(AbilityData.AbilityCommand);
    }
}

void UKYAbilitiesTabWidget::OnCategoryButtonClicked(const int32 ButtonIndex)
{
    ChangeSelectedCategory(ButtonIndex);
}

void UKYAbilitiesTabWidget::OnAbilityButtonClicked(const int32 ButtonIndex)
{
    ChangeSelectedAbility(ButtonIndex);
}


bool UKYAbilitiesTabWidget::HandleNavigationInput_Implementation(const float AxisX, const float AxisY)
{
    // 좌우 이동 처리
    if (FMath::Abs(AxisX) > 0.5f)
    {
        if (HandleHorizontalNavigation(AxisX))
        {
            return true;
        }
    }
    
    // 상하 이동 처리
    if (FMath::Abs(AxisY) > 0.5f)
    {
        return HandleVerticalNavigation(AxisY);
    }
    
    return false;
}

bool UKYAbilitiesTabWidget::HandleHorizontalNavigation(float AxisX)
{
    if (AxisX > 0 && NavigationMode == 0)
    {
        NavigationMode = 1;
        if (AbilityButtonWidgets.Num() > 0)
        {
            ChangeSelectedAbility(0);
        }
        KY_LOG(LogKY, Log, TEXT("Navigation Mode : %d"), NavigationMode);
        return true;
    }
    
    if (AxisX < 0 && NavigationMode == 1)
    {
        NavigationMode = 0;
        ClearAbilityDetails();
        return true;
    }
    return false;
}

bool UKYAbilitiesTabWidget::HandleVerticalNavigation(float AxisY)
{
    KY_LOG(LogKY, Log, TEXT("Navigation Mode : %d"), NavigationMode);
    if (NavigationMode == 0)
    {
        return HandleCategoryNavigation(AxisY);
    }
    return HandleAbilityNavigation(AxisY);
}

bool UKYAbilitiesTabWidget::HandleCategoryNavigation(float AxisY)
{
    const int32 NewIndex = AxisY < 0 ? 
        FMath::Max(0, SelectedCategoryIndex - 1) : 
        FMath::Min(CategoryButtonWidgets.Num() - 1, SelectedCategoryIndex + 1);

    KY_LOG(LogKY, Log, TEXT("SelectedCategoryIndex : %d, NewIndex : %d"), SelectedCategoryIndex, NewIndex);
    if (NewIndex != SelectedCategoryIndex)
    {
        ChangeSelectedCategory(NewIndex);
        return true;
    }
    return false;
}

bool UKYAbilitiesTabWidget::HandleAbilityNavigation(float AxisY)
{
    const int32 NewIndex = AxisY < 0 ? 
        FMath::Max(0, SelectedAbilityIndex - 1) : 
        FMath::Min(AbilityButtonWidgets.Num() - 1, SelectedAbilityIndex + 1);
    
    if (NewIndex != SelectedAbilityIndex)
    {
        ChangeSelectedAbility(NewIndex);
        return true;
    }
    return false;
}
