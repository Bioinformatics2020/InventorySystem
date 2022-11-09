// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BagGridWidgetTyp1.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include <Animation/WidgetAnimation.h>

#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "InventorySystem/EventManager/EventManager.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "UI/MoveGridItemDDO.h"

void UBagGridWidgetTyp1::BeginDestroy()
{
	Super::BeginDestroy();

	FEventManager::UnRegistEvent(FName("UpdateDotImage"), this);
	FEventManager::UnRegistEvent(FName(UpdateItemImageName), this);
	FEventManager::UnRegistEvent(FName(UpdateItemNumName), this);
}

void UBagGridWidgetTyp1::Init(FBagGridWidgetType1Attr BagGridWidgetType1Attr, int NewGridIndex, int NewItemNum, int NewItemID, bool bShowDot)
{
	GridIndex = NewGridIndex;
	//checkf(GridIndex < 1000 , TEXT("GridIndex<1000 %d %d"), GridIndex, NewItemID);

	SetVisibility(ESlateVisibility::Collapsed);
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);

	UpdateItemNum(0);

	UpdateItemImage(BagGridWidgetType1Attr, NewItemNum, NewItemID, bShowDot);


	WidgetSizeBox->SetWidthOverride(BagGridWidgetType1Attr.BagGridSize.X);
	WidgetSizeBox->SetHeightOverride(BagGridWidgetType1Attr.BagGridSize.Y);

	ADataAssetMananger::RequestAsyncLoadObjects(this,
		{ BagGridWidgetType1Attr.BorderTexture,BagGridWidgetType1Attr.BagGridBGTexture },
		[this](TArray<UObject*> Objects)
	{
		checkf(Objects.Num() == 2, TEXT("Load BagGridWidgetType1Attr Fail"));


		ItemBorder->Background.SetResourceObject(Objects[0]);
		BgBorder->Background.SetResourceObject(Objects[1]);

		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	);

	FEventManager::RegistEvent(FName("UpdateDotImage"), this, FName("UpdateDotImage"));

	UpdateItemImageName = "UpdateItemImage" + FString::FromInt(GridIndex);
	FEventManager::RegistEvent(FName(UpdateItemImageName), this, FName("UpdateItemImage"));

	UpdateItemNumName = "UpdateItemNum" + FString::FromInt(GridIndex);
	FEventManager::RegistEvent(FName(UpdateItemNumName), this, FName("UpdateItemNum"));
}

void UBagGridWidgetTyp1::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemID !=0)
	{
		SelectIamge->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayAnimation(ZoomAnim, 0, 0);
		if (DotImage->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			FEventManager::TriggerEvent(FName("ReadGridItem"), &ItemID);
		}
	}
	
}

void UBagGridWidgetTyp1::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemID != 0)
	{
		SelectIamge->SetVisibility(ESlateVisibility::Hidden);
		StopAnimation(ZoomAnim);
	}
	
}

FReply UBagGridWidgetTyp1::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemID != 0 && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}

	return FReply::Handled();
}

void UBagGridWidgetTyp1::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UMoveGridItemDDO* MoveGridItemDDO = Cast<UMoveGridItemDDO>(UWidgetBlueprintLibrary::CreateDragDropOperation(UMoveGridItemDDO::StaticClass()));
	MoveGridItemDDO->GridID = this->GridIndex;
	MoveGridItemDDO->ItemID = this->ItemID;
	MoveGridItemDDO->ItemNum = this->ItemNum;
	MoveGridItemDDO->Pivot = EDragPivot::MouseDown;
	UClass* MoveWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemMoveWidgetName);
	UWidget* MoveWidget = CreateWidget(GetOwningPlayer(), MoveWidgetClass);

	UFunction* InitFunc = MoveWidget->FindFunction(FName("Init"));
	checkf(InitFunc, TEXT("No Find Function:Init "));
	UObject* NewItemImage = ItemImage->Brush.GetResourceObject();
	MoveWidget->ProcessEvent(InitFunc, &NewItemImage);

	MoveGridItemDDO->DefaultDragVisual = MoveWidget;

	ItemImage->SetOpacity(0.5);

	MoveGridItemDDO->OnDragCancelled.AddDynamic(this, &UBagGridWidgetTyp1::DragCancel);
	MoveGridItemDDO->OnDrop.AddDynamic(this, &UBagGridWidgetTyp1::DragCancel);
	OutOperation = MoveGridItemDDO;
}

bool UBagGridWidgetTyp1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UMoveGridItemDDO* MoveGridItemDDO = Cast<UMoveGridItemDDO>(InOperation);
	if (MoveGridItemDDO)
	{
		struct
		{
			int GridID;
			int FromGridID;
		}Params;
		Params.GridID = this->GridIndex;
		Params.FromGridID = MoveGridItemDDO->GridID;
		//checkf(GridIndex < 1000 && MoveGridItemDDO->GridID < 1000, TEXT("GridIndex<1000 %d %d"), GridIndex, MoveGridItemDDO->GridID);
		FEventManager::TriggerEvent(FName("MoveGridItemToGridItem"), &Params);
	}

	return true;
}

void UBagGridWidgetTyp1::UpdateDotImage(int NewItemID)
{
	if (NewItemID == ItemID)
	{
		DotImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBagGridWidgetTyp1::UpdateItemImage(FBagGridWidgetType1Attr BagGridWidgetType1Attr, int NewItemNum, int NewItemID, bool bShowDot)
{
	if (NewItemID == 0)
	{
		UpdateItemNum(0);
		ItemMaxNum = 0;
		ItemID = 0;
		DotImage->SetVisibility(ESlateVisibility::Hidden);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		ItemImage->Brush.SetResourceObject(NULL);
		ItemBorder->SetToolTip(NULL);
	} 
	else if (NewItemID == ItemID)
	{
		ItemID = NewItemID;
		UpdateItemNum(NewItemNum);
	}
	else
	{
		ADataAssetMananger::RequestAsyncLoadObjects(this,
			{ BagGridWidgetType1Attr.ItemIamgeTexture },
			[this, NewItemNum, bShowDot](TArray<UObject*> Objects)
		{
			checkf(Objects.Num() == 1, TEXT("Load BagGridWidgetType1Attr Fail"));

			UpdateItemNum(NewItemNum);
			DotImage->SetVisibility(bShowDot ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
			ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ItemImage->Brush.SetResourceObject(Objects[0]);
		}
		);

		ADataAssetMananger::RequestSyncLoadClass(this, BagGridWidgetType1Attr.TipBorderWidgetClass);

		UClass* TipBorderWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, BagGridWidgetType1Attr.TipBorderWidgetClass);
		UClass* TipContextWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, BagGridWidgetType1Attr.TipContextWidgetClass);
		UUserWidget* TipBorderWidget = CreateWidget(GetWorld(), TipBorderWidgetClass);
		UUserWidget* TipContextWidget = CreateWidget(GetWorld(), TipContextWidgetClass);

		UFunction* InitFunc = TipBorderWidget->FindFunction(FName("Init"));
		checkf(InitFunc, TEXT("No Find Function:Init "));
		struct
		{
			int ID;
			int Num;
			UWidget* ContextWidget;
		}Params;
		Params.ID = NewItemID;
		Params.Num = NewItemNum;
		Params.ContextWidget = TipContextWidget;
		TipBorderWidget->ProcessEvent(InitFunc, &Params);

		ItemBorder->SetToolTip(TipBorderWidget);

		ItemMaxNum = BagGridWidgetType1Attr.MaxNum;
		ItemID = NewItemID;
		ItemMoveWidgetName = BagGridWidgetType1Attr.MoveItemWidgetType;
	}
	
}

void UBagGridWidgetTyp1::UpdateItemNum(int NewItemNum)
{
	ItemNum = NewItemNum;
	if (NewItemNum != 0)
	{
		UFunction* InitFunc = ItemBorder->ToolTipWidget->FindFunction(FName("UpData"));
		checkf(InitFunc, TEXT("No Find Function:UpData "));
		ItemBorder->ToolTipWidget->ProcessEvent(InitFunc, &ItemNum);
	}
	

	ItemNumText->SetText((ItemNum >= 2) ? FText::AsNumber(ItemNum) : FText::FromString(FString("")));

	FSlateColor NewColor((ItemNum == ItemMaxNum)?FLinearColor::Red:FLinearColor::White);
	ItemNumText->SetColorAndOpacity(NewColor);
}

void UBagGridWidgetTyp1::DragCancel(UDragDropOperation* Operation)
{
	ItemImage->SetOpacity(1);
}
