#pragma once
#include "CoreMinimal.h"
#include "Struct/KYStruct.h"
#include "UI/KYUserWidget.h"
#include "KYConfirmDialogWidget.generated.h"


class UListView;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogActionSelected, FName);

UCLASS()
class PROJECTKY_API UKYConfirmDialogWidget : public UKYUserWidget
{
public:
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	void SetupDialog(const TSharedPtr<FKYItemData>& ItemData);

	// 델리게이트로 결과 전달
	FOnDialogActionSelected OnActionSelected;

	// 리스트 선택 처리
	UFUNCTION()
	void HandleItemClicked(UObject* Item);

protected:
	UPROPERTY(meta = (BindWidget))
	UListView* ActionListView;

private:
	UPROPERTY() TArray<UObject*> EntryObjects;
};
