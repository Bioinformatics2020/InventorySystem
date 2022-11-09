// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BagWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UBagWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void Init();
	UFUNCTION()
		void OnCloseButtonChick();
	UFUNCTION()
		void OnSortButtonClick();
	UFUNCTION()
		void CreateAllBagGrid();
	UFUNCTION()
		void UpdataAllBagGrid();
	UFUNCTION()
		void UpdateOneBagGrid(int GridID);


	virtual void BeginDestroy() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* TitleName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* TitleBGImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* BagBGBorder;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* CloseButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* SortButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class USizeBox* WidgetSizeBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UWrapBox* BagBox;

	bool bHaveGridWidget;
};
