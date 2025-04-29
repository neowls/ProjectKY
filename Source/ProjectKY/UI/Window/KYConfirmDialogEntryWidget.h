#pragma once
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/KYUserWidget.h"
#include "KYConfirmDialogEntryWidget.generated.h"


class UTextBlock;

UCLASS()
class UKYConfirmDialogEntryWidget : public UKYUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LabelText;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override; 
};
