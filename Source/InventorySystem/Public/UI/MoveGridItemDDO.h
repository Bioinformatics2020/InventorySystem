// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "MoveGridItemDDO.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UMoveGridItemDDO : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int GridID;
	int ItemID;
	int ItemNum;
};
