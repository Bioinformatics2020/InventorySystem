// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUBWidget.h"
#include <Misc/ConfigCacheIni.h>
#include <Components/HorizontalBox.h>
#include <Components/NamedSlot.h>
#include <Kismet/KismetSystemLibrary.h>

#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "InventorySystem/EventManager/EventManager.h"
#include "DataTableToolLibrary.h"
#include "UI/MoveGridItemDDO.h"

void UHUBWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<FString> NavButtonStrs;
	GConfig->GetArray(TEXT("GameUIInit/HUBWidget/NavgationButtons"), TEXT("NavgationButtons"), NavButtonStrs, GGameIni);

	for (FString NavButtonStr: NavButtonStrs)
	{
		FNavButtonAttr* NavButtonAttrPtr = UDataTableToolLibrary::GetNavButtonAttr(FName(*NavButtonStr));
		checkf(NavButtonAttrPtr, TEXT("DataTable is Null"));
		FNavButtonAttr NavButtonAttr = *NavButtonAttrPtr;
		FEventManager::RegistEvent(NavButtonAttr.OnClickedEvent, this, NavButtonAttr.OnClickedEvent);

		ADataAssetMananger::RequestAsyncLoadClass(
			this,
			NavButtonAttr.UIType,
			[this,NavButtonAttr](UClass* WidgetClass) {
				UWidget* ButtonWidget = CreateWidget(this->GetOwningPlayer(), WidgetClass);

				NavButtonPanel->AddChildToHorizontalBox(ButtonWidget);

				UFunction* InitFunc = ButtonWidget->FindFunction(FName("Init"));
				if (InitFunc)
				{
					struct
					{
						FNavButtonAttr NewNavButtonAttr;
					}Param;
					Param.NewNavButtonAttr = NavButtonAttr;
					ButtonWidget->ProcessEvent(InitFunc, &Param);
				}
			}
		);
	}
}

void UHUBWidget::NativeDestruct()
{

	Super::NativeDestruct();
}

bool UHUBWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (UMoveGridItemDDO* MoveGridItemDDO = Cast<UMoveGridItemDDO>(InOperation))
	{
		AActor* OwnerPawn = GetOwningPlayer()->GetPawn();
		FItemOnGroundData NewItemOnGround;
		NewItemOnGround.ID = MoveGridItemDDO->ItemID;
		NewItemOnGround.Num = MoveGridItemDDO->ItemNum;
		NewItemOnGround.Location = OwnerPawn->GetActorLocation() + OwnerPawn->GetActorForwardVector() * 100 + OwnerPawn->GetActorRightVector() * 100 + OwnerPawn->GetActorUpVector() * 100;
		FEventManager::TriggerEvent(FName("AddOneItemOnGround"), &NewItemOnGround);
		FEventManager::TriggerEvent(FName("SubGridItem"), &MoveGridItemDDO->GridID);
	}
	return false;
}

void UHUBWidget::OpenBagWidget()
{
	FString BagWidgetstr;
	GConfig->GetString(TEXT("GameUIInit/BagWidgetInit"), TEXT("BagWidgetType"), BagWidgetstr, GGameIni);
	UClass* BagWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, *BagWidgetstr);
	BagWidget = CreateWidget(GetOwningPlayer(), BagWidgetClass);

	UFunction* InitFunc = BagWidget->FindFunction(FName("Init"));
	checkf(InitFunc, TEXT("No Find Function:Init "));
	BagWidget->ProcessEvent(InitFunc, NULL);

	FEventManager::RegistEvent(FName("CloseBagWidget"), this, FName("CloseBagWidget"));

	ShowBagSlot->AddChild(BagWidget);
}

void UHUBWidget::CloseBagWidget()
{
	BagWidget->RemoveFromParent();
	BagWidget = NULL;
	FEventManager::UnRegistEvent(FName("CloseBagWidget"), this);
}

void UHUBWidget::ToggleSettingEvent()
{
	UKismetSystemLibrary::PrintString(this, TEXT(__FUNCTION__));
}

void UHUBWidget::ToggleBagEvent()
{
	if (IsValid(BagWidget))
	{
		CloseBagWidget();
	}
	else
	{
		OpenBagWidget();
	}
}
