// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructTypes.h"
#include "BagGridWidgetTyp1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UBagGridWidgetTyp1 : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void BeginDestroy() override;

	UFUNCTION()
		void Init(FBagGridWidgetType1Attr BagGridWidgetType1Attr, int NewGridIndex, int NewItemNum, int NewItemID, bool bShowDot);

	UFUNCTION()
		void UpdateDotImage(int NewItemID);

	UFUNCTION()
		void UpdateItemImage(FBagGridWidgetType1Attr BagGridWidgetType1Attr, int NewItemNum, int NewItemID, bool bShowDot);
	UFUNCTION()
		void UpdateItemNum(int ItemNum);

	UFUNCTION()
		void DragCancel(UDragDropOperation* Operation);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* ItemNumText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* BgBorder; 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* ItemBorder;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* ItemImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* SelectIamge;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* DotImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class USizeBox* WidgetSizeBox;

	UPROPERTY(Transient,meta = (BindWidgetAnim))
		class UWidgetAnimation* ZoomAnim;

protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:

	int GridIndex;
	int ItemID;
	int ItemNum;
	int ItemMaxNum;
	FString UpdateItemImageName;
	FString UpdateItemNumName;
	FName ItemMoveWidgetName;
};
