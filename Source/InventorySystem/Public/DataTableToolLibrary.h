// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StructTypes.h"
#include "DataTableToolLibrary.generated.h"

#define DATATABLE_Signed(DataType) \
static F##DataType##* Get##DataType(FName RowName);\
static UDataTable* DataType##DT;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDataTableToolLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DATATABLE_Signed(NavButtonAttr);
	DATATABLE_Signed(BagWidgetType1Attr);
	DATATABLE_Signed(BagGridWidgetType1Attr);
	DATATABLE_Signed(ItemOnGroundAttr);
	DATATABLE_Signed(ItemTipBorderAttr);
};
