// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BagWidgetType1.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include <Components/WrapBox.h>
#include <GameFramework/PlayerController.h>

#include "DataTableToolLibrary.h"
#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "InventoryPlayerState.h"
#include "InventorySystem/EventManager/EventManager.h"

void UBagWidgetType1::Init()
{
	//Type1
	FString BagWidgetstr;
	GConfig->GetString(TEXT("GameUIInit/BagWidgetInit"), TEXT("BagWidgetType1"), BagWidgetstr, GGameIni);
	SetVisibility(ESlateVisibility::Collapsed);
	FBagWidgetType1Attr* BagWidgetType1AttrPtr = UDataTableToolLibrary::GetBagWidgetType1Attr(FName(*BagWidgetstr));
	checkf(BagWidgetType1AttrPtr, TEXT("BagWidgetType1Attr is NULL"));
	FBagWidgetType1Attr BagWidgetType1Attr = *BagWidgetType1AttrPtr;
	TitleName->SetText(FText::FromName(BagWidgetType1Attr.TitleName));
	WidgetSizeBox->SetWidthOverride(BagWidgetType1Attr.BagSize.X);
	WidgetSizeBox->SetHeightOverride(BagWidgetType1Attr.BagSize.Y);

	checkf(BagWidgetType1Attr.ButtonIcons.Num() == 3, TEXT("Load CloseButtonIcons Fail"));
	TArray<FName> Icons = BagWidgetType1Attr.ButtonIcons;
	Icons.Add(BagWidgetType1Attr.TitleTexture);
	Icons.Add(BagWidgetType1Attr.BagBGTextue);

	CloseButton->OnClicked.AddDynamic(this, &UBagWidgetType1::OnCloseButtonChick);
	SortButton->OnClicked.AddDynamic(this, &UBagWidgetType1::OnSortButtonClick);

	ADataAssetMananger::RequestAsyncLoadObjects(this, 
		Icons,
		[this](TArray<UObject*> Objects)
		{
			checkf(Objects.Num() == 5, TEXT("Load TitleTexture or BagBGTextue Fail"));

			CloseButton->WidgetStyle.Pressed.SetResourceObject(Objects[0]);
			CloseButton->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
			CloseButton->WidgetStyle.Hovered.SetResourceObject(Objects[1]);
			CloseButton->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
			CloseButton->WidgetStyle.Normal.SetResourceObject(Objects[2]);
			CloseButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

			TitleBGImage->Brush.SetResourceObject(Objects[3]);
			BagBGBorder->Background.SetResourceObject(Objects[4]);

			SetVisibility(ESlateVisibility::Visible);
		}
	);
	
	AInventoryPlayerState* PlayerState = Cast<AInventoryPlayerState>(GetOwningPlayer<APlayerController>()->GetPlayerState<APlayerState>());
	const TArray<FBagGridData>& BagGridData = PlayerState->GetGridDatas();
	for (int i = 0; i < BagGridData.Num(); ++i)
	{
		UClass* BagGridWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, TEXT("BagGridWidgetType1"));
		check(BagGridWidgetClass);
		UUserWidget* BagGridWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), BagGridWidgetClass);
		BagBox->AddChildToWrapBox(BagGridWidget);
	}

	CreateAllBagGrid();

	FEventManager::RegistEvent(FName("UpdataAllBagGrid"), this, FName("UpdataAllBagGrid"));
	FEventManager::RegistEvent(FName("UpdateOneBagGrid"), this, FName("UpdateOneBagGrid"));
	
}

void UBagWidgetType1::OnCloseButtonChick()
{
	FEventManager::TriggerEvent(FName("CloseBagWidget"),NULL);
}

void UBagWidgetType1::OnSortButtonClick()
{
	FEventManager::TriggerEvent(FName("SortBagGrid"), NULL);
}

void UBagWidgetType1::CreateAllBagGrid()
{
	AInventoryPlayerState* PlayerState = Cast<AInventoryPlayerState>(GetOwningPlayer<APlayerController>()->GetPlayerState<APlayerState>());
	const TArray<FBagGridData>& BagGridData = PlayerState->GetGridDatas();
	TArray<UWidget*> GridWidgets = BagBox->GetAllChildren();
	
	struct
	{
		FBagGridWidgetType1Attr BagGridWidgetType1Attr;
		int ItemIndex;
		int ItemNum;
		int ItemID;
		bool bShowDot;
	}Params;

	for (int GridID = 0; GridID < GridWidgets.Num(); ++GridID)
	{
		FBagGridWidgetType1Attr* BagGridWidgetType1AttrPtr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagGridData[GridID].ID)));
		checkf(BagGridWidgetType1AttrPtr, TEXT("BagGridWidgetType1Attr is NULL"));

		UFunction* InitFunc = GridWidgets[GridID]->FindFunction(FName("Init"));
		checkf(InitFunc, TEXT("No Find Function:Init "));
		
		Params.ItemIndex = GridID;
		Params.ItemNum = BagGridData[GridID].Num;
		Params.ItemID = BagGridData[GridID].ID;
		Params.bShowDot = !PlayerState->PlayerHaveItem(BagGridData[GridID].ID);
		Params.BagGridWidgetType1Attr = *BagGridWidgetType1AttrPtr;
		GridWidgets[GridID]->ProcessEvent(InitFunc, &Params);
	}
}

void UBagWidgetType1::UpdataAllBagGrid()
{
	AInventoryPlayerState* PlayerState = Cast<AInventoryPlayerState>(GetOwningPlayer<APlayerController>()->GetPlayerState<APlayerState>());
	const TArray<FBagGridData>& BagGridData = PlayerState->GetGridDatas();

	struct
	{
		FBagGridWidgetType1Attr BagGridWidgetType1Attr;
		int NewItemNum;
		int NewItemID;
		bool bShowDot;
	}Params;

	for (int GridID = 0; GridID < BagBox->GetAllChildren().Num(); ++GridID)
	{
		FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagGridData[GridID].ID)));

		Params.BagGridWidgetType1Attr = *BagGridWidgetType1Attr;
		Params.NewItemNum = BagGridData[GridID].Num;
		Params.NewItemID = BagGridData[GridID].ID;
		Params.bShowDot = !PlayerState->PlayerHaveItem(BagGridData[GridID].ID);
		FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(GridID)), &Params);
	}
}

void UBagWidgetType1::UpdateOneBagGrid(int GridID)
{
	AInventoryPlayerState* PlayerState = Cast<AInventoryPlayerState>(GetOwningPlayer<APlayerController>()->GetPlayerState<APlayerState>());
	const TArray<FBagGridData>& BagGridData = PlayerState->GetGridDatas();
	FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagGridData[GridID].ID)));

	struct
	{
		FBagGridWidgetType1Attr BagGridWidgetType1Attr;
		int NewItemNum;
		int NewItemID;
		bool bShowDot;
	}Params;

	Params.BagGridWidgetType1Attr = *BagGridWidgetType1Attr;
	Params.NewItemNum = BagGridData[GridID].Num;
	Params.NewItemID = BagGridData[GridID].ID;
	Params.bShowDot = !PlayerState->PlayerHaveItem(BagGridData[GridID].ID);
	FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(GridID)), &Params);
}

void UBagWidgetType1::BeginDestroy()
{
	FEventManager::UnRegistEvent(FName("UpdataAllBagGrid"), this);
	FEventManager::UnRegistEvent(FName("UpdateOneBagGrid"), this);
	Super::BeginDestroy();
}

bool UBagWidgetType1::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}
