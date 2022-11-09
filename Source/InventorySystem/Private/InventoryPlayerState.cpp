// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPlayerState.h"
#include "FileTool.h"
#include "DataTableToolLibrary.h"
#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "InventorySystem/EventManager/EventManager.h"

void AInventoryPlayerState::BeginPlay()
{
 	Super::BeginPlay();

	FFileTool::LoadGameSaveDataInFile(GameSaveData);
	CreateOnGroundItem();

	FEventManager::RegistEvent(FName("TryPickUpItemOnGround"), this, FName("TryPickUpItemOnGround"));
	FEventManager::RegistEvent(FName("OverPickUpItemOnGround"), this, FName("OverPickUpItemOnGround"));
	FEventManager::RegistEvent(FName("AddOneItemOnGround"), this, FName("AddOneItemOnGround"));
}

void AInventoryPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveGameSaveData();
	FEventManager::UnRegistEvent(FName("TryPickUpItemOnGround"), this);
	FEventManager::UnRegistEvent(FName("OverPickUpItemOnGround"), this);
	FEventManager::UnRegistEvent(FName("AddOneItemOnGround"), this);
	Super::EndPlay(EndPlayReason);
}

void AInventoryPlayerState::SaveGameSaveData()
{
	FFileTool::SaveGameSaveDataToFile(GameSaveData);
}

void AInventoryPlayerState::TryPickUpItemOnGround(FName Index)
{
	FItemOnGroundData* ItemOnGroundData = GameSaveData.GroundData.ItemOnGroundDatas.Find(Index);
	check(ItemOnGroundData);
	int Surplus = ItemOnGroundData->Num;
	if (OnTryAddItemOnGroundDelegate.IsBound())
	{
		Surplus = OnTryAddItemOnGroundDelegate.Execute(ItemOnGroundData->ID, ItemOnGroundData->Num);
	}

	if (Surplus>0)
	{
		ItemOnGroundData->Num = Surplus;
	}
	else
	{
		GameSaveData.GroundData.ItemOnGroundDatas.Remove(Index);
	}
	struct
	{
		int Num;
		FName Index;
	}Params;
	Params.Index = Index;
	Params.Num = Surplus;
	FEventManager::TriggerEvent(FName("EndPickUp"), &Params);
}

void AInventoryPlayerState::OverPickUpItemOnGround(FName Index, FVector Location)
{
	FItemOnGroundData* ItemOnGroundData = GameSaveData.GroundData.ItemOnGroundDatas.Find(Index);
	if (ItemOnGroundData)
	{
		ItemOnGroundData->Location = Location;
	}
}

void AInventoryPlayerState::AddOneItemOnGround(FItemOnGroundData NewItemOnGround)
{
	FItemOnGroundAttr* ItemOnGroundAttr = UDataTableToolLibrary::GetItemOnGroundAttr(FName(FString::FromInt(NewItemOnGround.ID)));

	UClass* ActorClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundAttr->ActorType);

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* ItemActor = GetWorld()->SpawnActor<AActor>(ActorClass, NewItemOnGround.Location, FRotator(), ActorSpawnParameters);

	
	FString IndexStr = FString::FromInt(NewItemOnGround.ID);
	int i = 0;
	while (GameSaveData.GroundData.ItemOnGroundDatas.Find(FName(IndexStr + "_" + FString::FromInt(i))))
		i++;
	FName Index(IndexStr + "_" + FString::FromInt(i));

	UFunction* InitFunc = ItemActor->FindFunction(FName("Init"));
	if (InitFunc)
	{
		struct
		{
			int ID;
			int Num;
			FName Index;
		}Params;
		Params.ID = NewItemOnGround.ID;
		Params.Num = NewItemOnGround.Num;
		Params.Index = Index;
		ItemActor->ProcessEvent(InitFunc, &Params);
	}

	GameSaveData.GroundData.ItemOnGroundDatas.Add(Index, NewItemOnGround);
}

FGameSaveData& AInventoryPlayerState::GetGameSaveData()
{
	return GameSaveData;
}

const TArray<FBagGridData>& AInventoryPlayerState::GetGridDatas()
{
	return GameSaveData.PlayerData.BagData.GridDatas;
}

bool AInventoryPlayerState::PlayerHaveItem(int ItemID)
{
	return GameSaveData.PlayerData.BagData.HadItems.Contains(ItemID);
}

void AInventoryPlayerState::CreateOnGroundItem()
{
	
	for (TPair<FName, FItemOnGroundData> Pair:GameSaveData.GroundData.ItemOnGroundDatas)
	{
		FItemOnGroundAttr* ItemOnGroundAttr = UDataTableToolLibrary::GetItemOnGroundAttr(FName(FString::FromInt(Pair.Value.ID)));

		UClass* ActorClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundAttr->ActorType);

		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* ItemActor = GetWorld()->SpawnActor<AActor>(ActorClass, Pair.Value.Location, FRotator(), ActorSpawnParameters);

		UFunction* InitFunc = ItemActor->FindFunction(FName("Init"));
		if (InitFunc)
		{
			struct
			{
				int ID;
				int Num;
				FName Index;
			}Params;
			Params.ID = Pair.Value.ID;
			Params.Num = Pair.Value.Num;
			Params.Index = Pair.Key;
			ItemActor->ProcessEvent(InitFunc, &Params);
		}
	}
}
