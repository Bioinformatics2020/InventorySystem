// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "StructTypes.h"
#include "InventoryPlayerState.generated.h"

DECLARE_DELEGATE_RetVal_TwoParams(int,FTryAddItemOnGroundDelegate,int,int)

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable)
		void SaveGameSaveData();

	UFUNCTION()
		void TryPickUpItemOnGround(FName Index);
	UFUNCTION()
		void OverPickUpItemOnGround(FName Index, FVector Location);
	UFUNCTION()
		void AddOneItemOnGround(FItemOnGroundData NewItemOnGround);


	FGameSaveData& GetGameSaveData();
	const TArray<FBagGridData>& GetGridDatas();
	bool PlayerHaveItem(int ItemID);

	void CreateOnGroundItem();

	FTryAddItemOnGroundDelegate OnTryAddItemOnGroundDelegate;

private:
	UPROPERTY(EditAnywhere)
		FGameSaveData GameSaveData;
};
