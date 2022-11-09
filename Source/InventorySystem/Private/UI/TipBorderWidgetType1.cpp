// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TipBorderWidgetType1.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include <Components/NamedSlot.h>
#include "DataTableToolLibrary.h"

void UTipBorderWidgetType1::Init(int ID, int Num, UWidget* ContextWidget)
{
	FItemTipBorderAttr* BagWidgetType1AttrPtr = UDataTableToolLibrary::GetItemTipBorderAttr(FName(*FString::FromInt(ID)));

	ChildContextWidget = ContextWidget;
	ContextSlot->AddChild(ContextWidget);
	TitleName->SetText(FText::FromName(BagWidgetType1AttrPtr->Name));

	UFunction* InitFunc = ChildContextWidget->FindFunction(FName("Init"));
	checkf(InitFunc, TEXT("No Find Function:Init "));
	struct
	{
		FName Info;
		int Num;
	}Params;
	Params.Info = BagWidgetType1AttrPtr->Info;
	Params.Num = Num;
	ChildContextWidget->ProcessEvent(InitFunc, &Params);
}

void UTipBorderWidgetType1::UpData(int Num)
{
	UFunction* InitFunc = ChildContextWidget->FindFunction(FName("UpData"));
	checkf(InitFunc, TEXT("No Find Function:UpData "));
	ChildContextWidget->ProcessEvent(InitFunc, &Num);
}

