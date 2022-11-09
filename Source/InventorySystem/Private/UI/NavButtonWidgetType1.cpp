// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NavButtonWidgetType1.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"

#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "InventorySystem/EventManager/EventManager.h"

void UNavButtonWidgetType1::Init(FNavButtonAttr NavButtonAttr)
{
	HotKeyText->SetText(FText::FromName(NavButtonAttr.HotKey));
	SetVisibility(ESlateVisibility::Collapsed);
	ButtonSizeBox->SetHeightOverride(NavButtonAttr.Size);
	ButtonSizeBox->SetWidthOverride(NavButtonAttr.Size);
	if (UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Slot))
	{
		HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
	}
	
	NavButton->OnClicked.AddDynamic(this, &UNavButtonWidgetType1::OnChickedFunc);
	ChickEventName = NavButtonAttr.OnClickedEvent;
	ADataAssetMananger::RequestAsyncLoadObjects(this,NavButtonAttr.ButtonIcons,
		[this](TArray<UObject*> Objects)
		{
			checkf(Objects.Num() == 3, TEXT("Load ButtonIcons Fail"));
			NavButton->WidgetStyle.Pressed.SetResourceObject(Objects[0]);
			NavButton->WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::Image;
			NavButton->WidgetStyle.Hovered.SetResourceObject(Objects[1]);
			NavButton->WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::Image;
			NavButton->WidgetStyle.Normal.SetResourceObject(Objects[2]);
			NavButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Image;

			SetVisibility(ESlateVisibility::Visible);
		}
	);
}

void UNavButtonWidgetType1::OnChickedFunc()
{
	FEventManager::TriggerEvent(ChickEventName, NULL);
}
