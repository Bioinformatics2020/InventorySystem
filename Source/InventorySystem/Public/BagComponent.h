// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BagComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UBagComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBagComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		int AddItem(int ItemID, int ItemNum);
	int HaveItemNum(int ItemID);
	UFUNCTION(BlueprintCallable)
		bool SubItem(int ItemID, int ItemNum);

	UFUNCTION()
		void ReadGridItem(int ItemID);

	UFUNCTION()
		void SortBagGrid();

	UFUNCTION()
		void MoveGridItemToGridItem(int SourceItemGridIndex, int FromItemGridIndex);
	UFUNCTION()
		void SubGridItem(int ItemGridIndex);
		
	struct FBagData* BagData;
};
