#include "KYConfirmDialogEntryWidget.h"
#include "Components/TextBlock.h"
#include "Data/KYConfirmDialogEntryObject.h"

void UKYConfirmDialogEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (const auto* Entry = Cast<UKyConfirmDialogEntryObject>(ListItemObject))
	{
		LabelText->SetText(Entry->DisplayText);
	}
}
