// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUBWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UHUBWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void OpenBagWidget();
	UFUNCTION()
		void CloseBagWidget();

	UFUNCTION()
		void ToggleSettingEvent();
	UFUNCTION()
		void ToggleBagEvent();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UHorizontalBox* NavButtonPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UNamedSlot* ShowBagSlot;

	class UUserWidget* BagWidget;
};
