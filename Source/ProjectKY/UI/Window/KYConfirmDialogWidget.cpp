#include "KYConfirmDialogWidget.h"

#include "Components/ListView.h"
#include "Data/KYConfirmDialogEntryObject.h"

void UKYConfirmDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ActionListView->OnItemClicked().AddUObject(this, &UKYConfirmDialogWidget::HandleItemClicked);
}

void UKYConfirmDialogWidget::SetupDialog(const TSharedPtr<FKYItemData>& ItemData)
{
	EntryObjects.Reset();

	auto CreateEntry = [](const FName& ID, const FText& Text)-> UObject*
	{
		auto* Entry = NewObject<UKyConfirmDialogEntryObject>();
		Entry->ActionID = ID;
		Entry->DisplayText = Text;
		return Entry;
	};

	if (ItemData->ItemType == EKYItemType::Armor || ItemData->ItemType == EKYItemType::Weapon)
	{
		EntryObjects.Add(CreateEntry("Equip", FText::FromString(TEXT("Equip"))));
	}
	else if (ItemData->ItemType == EKYItemType::Usable)
	{
		EntryObjects.Add(CreateEntry("Use", FText::FromString(TEXT("Use"))));
	}

	EntryObjects.Add(CreateEntry("Drop", FText::FromString(TEXT("Drop"))));
	EntryObjects.Add(CreateEntry("Cancel", FText::FromString(TEXT("Cancel"))));

	ActionListView->SetListItems(EntryObjects);
}

void UKYConfirmDialogWidget::HandleItemClicked(UObject* Item)
{
	if (const auto* Entry = Cast<UKyConfirmDialogEntryObject>(Item))
	{
		OnActionSelected.Broadcast(Entry->ActionID);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
