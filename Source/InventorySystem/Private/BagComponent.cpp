// Fill out your copyright notice in the Description page of Project Settings.


#include "BagComponent.h"

#include "InventoryPlayerState.h"
#include "InventorySystem/EventManager/EventManager.h"
#include "DataTableToolLibrary.h"

// Sets default values for this component's properties
UBagComponent::UBagComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBagComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	AInventoryPlayerState* PlayerState = Cast<AInventoryPlayerState>(Cast<APawn>(GetOwner())->GetPlayerState<APlayerState>());
	BagData = &PlayerState->GetGameSaveData().PlayerData.BagData;
	PlayerState->OnTryAddItemOnGroundDelegate.BindUFunction(this, FName("AddItem"));

	FEventManager::RegistEvent(FName("ReadGridItem"), this, FName("ReadGridItem"));
	FEventManager::RegistEvent(FName("SortBagGrid"), this, FName("SortBagGrid"));
	FEventManager::RegistEvent(FName("MoveGridItemToGridItem"), this, FName("MoveGridItemToGridItem"));
	FEventManager::RegistEvent(FName("SubGridItem"), this, FName("SubGridItem"));
	
}


void UBagComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FEventManager::UnRegistEvent(FName("ReadGridItem"),this);
	FEventManager::UnRegistEvent(FName("SortBagGrid"),this);
	FEventManager::UnRegistEvent(FName("MoveGridItemToGridItem"),this);
	FEventManager::UnRegistEvent(FName("SubGridItem"),this);

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UBagComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UBagComponent::AddItem(int ItemID, int ItemNum)
{
	FBagGridWidgetType1Attr* ItemData = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(ItemID)));
	if (ItemData == NULL)
	{
		return ItemNum;
	}
	int MaxNum = ItemData->MaxNum;
	FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(ItemID)));

	while (ItemNum > 0)
	{
		int MinGrid = -1;
		int MinValue = -1;
		for (int i = 0; i < BagData->GridDatas.Num(); ++i)
		{
			if (BagData->GridDatas[i].ID == ItemID && BagData->GridDatas[i].Num > MinValue && BagData->GridDatas[i].Num < MaxNum)
			{
				MinValue = BagData->GridDatas[i].Num;
				MinGrid = i;
			}
		}
		if (MinGrid != -1)
		{
			if (MaxNum - BagData->GridDatas[MinGrid].Num >= ItemNum)
			{
				BagData->GridDatas[MinGrid].Num += ItemNum;
				FEventManager::TriggerEvent(FName("UpdateItemNum" + FString::FromInt(MinGrid)), &BagData->GridDatas[MinGrid].Num);
				return 0;
			}
			else
			{
				ItemNum -= (MaxNum - BagData->GridDatas[MinGrid].Num);
				BagData->GridDatas[MinGrid].Num = MaxNum;
				FEventManager::TriggerEvent(FName("UpdateItemNum" + FString::FromInt(MinGrid)), &MaxNum);
				continue;
			}
		}
		break;
	}

	while (ItemNum > 0)
	{
		int MinGrid = -1;
		for (int i = BagData->GridDatas.Num()-1; i >= 0; --i)
		{
			if (BagData->GridDatas[i].ID == 0)
			{
				MinGrid = i;
			}
		}
		if (MinGrid != -1)
		{
			struct
			{
				FBagGridWidgetType1Attr BagGridWidgetType1Attr;
				int NewItemNum;
				int NewItemID;
				bool bShowDot;
			}Params;
			Params.BagGridWidgetType1Attr = *BagGridWidgetType1Attr;
			//Params.NewItemNum;
			Params.NewItemID = ItemID;
			Params.bShowDot = !BagData->HadItems.Contains(ItemID);

			if (MaxNum >= ItemNum)
			{
				Params.NewItemNum = ItemNum;
				BagData->GridDatas[MinGrid].Num = ItemNum;
				BagData->GridDatas[MinGrid].ID = ItemID;
				FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(MinGrid)), &Params);
				return 0;
			}
			else
			{
				ItemNum -= MaxNum;
				Params.NewItemNum = MaxNum;
				BagData->GridDatas[MinGrid].Num = MaxNum;
				BagData->GridDatas[MinGrid].ID = ItemID;
				FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(MinGrid)), &Params);
				continue;
			}
		}
		break;
	}

	return ItemNum;
}

int UBagComponent::HaveItemNum(int ItemID)
{
	int SumItemNum = 0;
	for (FBagGridData GridData : BagData->GridDatas)
	{
		if (GridData.ID == ItemID)
		{
			SumItemNum += GridData.Num;
		}
	}
	return SumItemNum;
}

bool UBagComponent::SubItem(int ItemID, int ItemNum)
{
	if (HaveItemNum(ItemID) < ItemNum)
		return false;

	int MaxNum = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(ItemID)))->MaxNum;
	FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(ItemID)));

	while (ItemNum > 0)
	{
		int MinGrid = -1;
		int MinValue = INT_MAX;
		for (int i = 0; i < BagData->GridDatas.Num(); ++i)
		{
			if (BagData->GridDatas[i].ID == ItemID && BagData->GridDatas[i].Num < MinValue)
			{
				MinValue = BagData->GridDatas[i].Num;
				MinGrid = i;
			}
		}
		if (MinGrid != -1)
		{
			if (BagData->GridDatas[MinGrid].Num > ItemNum)
			{
				int temp = BagData->GridDatas[MinGrid].Num - ItemNum;
				BagData->GridDatas[MinGrid].Num = temp;
				FEventManager::TriggerEvent(FName("UpdateItemNum" + FString::FromInt(MinGrid)), &temp);
				return true;
			}
			else
			{
				ItemNum -= BagData->GridDatas[MinGrid].Num;
				struct
				{
					FBagGridWidgetType1Attr BagGridWidgetType1Attr;
					int NewItemNum;
					int NewItemID;
					bool bShowDot;
				}Params;
				Params.NewItemID = 0;
				BagData->GridDatas[MinGrid].Num = 0;
				BagData->GridDatas[MinGrid].ID = 0;
				FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(MinGrid)), &Params);
				continue;
			}
		}
		break;
	}

	return !ItemNum;
}

void UBagComponent::ReadGridItem(int ItemID)
{
	BagData->HadItems.Add(ItemID);

	FEventManager::TriggerEvent(FName("UpdateDotImage"), &ItemID);
}

void UBagComponent::SortBagGrid()
{
	TMap<int, int> Map;
	for (int i = 0; i < BagData->GridDatas.Num(); ++i)
	{
		if (BagData->GridDatas[i].ID != 0)
		{
			int& MapValue = Map.FindOrAdd(BagData->GridDatas[i].ID);
			MapValue += BagData->GridDatas[i].Num;
			BagData->GridDatas[i].Num = 0;
			BagData->GridDatas[i].ID = 0;
		}
	}
	
	Map.KeySort([](int a, int b) {return a >= b; });

	int i = 0;
	for (TPair<int,int> Pair : Map)
	{
		int MaxNum = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(Pair.Key)))->MaxNum;
		while (Pair.Value>0)
		{
			BagData->GridDatas[i].ID = Pair.Key;
			if (Pair.Value>MaxNum)
			{
				BagData->GridDatas[i].Num = MaxNum;
				Pair.Value -= MaxNum;
			}
			else
			{
				BagData->GridDatas[i].Num = Pair.Value;
				Pair.Value = 0;
			}
			i++;
		}
	}

	FEventManager::TriggerEvent(FName("UpdataAllBagGrid"), NULL);
}

void UBagComponent::MoveGridItemToGridItem(int TargrtItemGridIndex, int FromItemGridIndex)
{
	checkf(TargrtItemGridIndex < 1000 && FromItemGridIndex < 1000, TEXT("GridIndex<1000 %d %d"), TargrtItemGridIndex, FromItemGridIndex);
	FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagData->GridDatas[FromItemGridIndex].ID)));
	FBagGridWidgetType1Attr* FromBagGridWidgetType1Attr = BagGridWidgetType1Attr;

	if (TargrtItemGridIndex == FromItemGridIndex)
	{
		return;
	}
	if (BagData->GridDatas[TargrtItemGridIndex].ID != BagData->GridDatas[FromItemGridIndex].ID)
	{
		FromBagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagData->GridDatas[TargrtItemGridIndex].ID)));
		
		FBagGridData Temp = BagData->GridDatas[TargrtItemGridIndex];
		BagData->GridDatas[TargrtItemGridIndex] = BagData->GridDatas[FromItemGridIndex];
		BagData->GridDatas[FromItemGridIndex] = Temp;
	}
	else
	{
		int SumNum = BagData->GridDatas[TargrtItemGridIndex].Num + BagData->GridDatas[FromItemGridIndex].Num;
		if (SumNum <= BagGridWidgetType1Attr->MaxNum)
		{
			BagData->GridDatas[FromItemGridIndex].ID = 0;
			BagData->GridDatas[FromItemGridIndex].Num = 0;
			BagData->GridDatas[TargrtItemGridIndex].Num = SumNum;
		}
		else
		{
			BagData->GridDatas[FromItemGridIndex].Num = SumNum - BagGridWidgetType1Attr->MaxNum;
			BagData->GridDatas[TargrtItemGridIndex].Num = BagGridWidgetType1Attr->MaxNum;
		}
	}

	FEventManager::TriggerEvent("UpdateOneBagGrid", &TargrtItemGridIndex);
	FEventManager::TriggerEvent("UpdateOneBagGrid", &FromItemGridIndex);
}

void UBagComponent::SubGridItem(int ItemGridIndex)
{
	BagData->GridDatas[ItemGridIndex].ID = 0;
	BagData->GridDatas[ItemGridIndex].Num = 0;

	struct
	{
		FBagGridWidgetType1Attr BagGridWidgetType1Attr;
		int NewItemNum;
		int NewItemID;
		bool bShowDot;
	}Params;
	FBagGridWidgetType1Attr* BagGridWidgetType1Attr = UDataTableToolLibrary::GetBagGridWidgetType1Attr(FName(*FString::FromInt(BagData->GridDatas[ItemGridIndex].ID)));
	Params.BagGridWidgetType1Attr = *BagGridWidgetType1Attr;
	Params.NewItemNum = BagData->GridDatas[ItemGridIndex].Num;
	Params.NewItemID = BagData->GridDatas[ItemGridIndex].ID;
	Params.bShowDot = !BagData->HadItems.Contains(BagData->GridDatas[ItemGridIndex].ID);
	FEventManager::TriggerEvent(FName("UpdateItemImage" + FString::FromInt(ItemGridIndex)), &Params);
}

