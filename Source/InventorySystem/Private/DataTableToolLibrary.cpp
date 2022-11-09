// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableToolLibrary.h"
#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"

#define DATATABLE_Impl(DataType,DataTablePath) \
\
UDataTable* UDataTableToolLibrary::##DataType##DT = NULL;\
\
F##DataType* UDataTableToolLibrary::Get##DataType##(FName RowName)\
{\
	if (DataType##DT == NULL)\
	{\
		DataType##DT = LoadObject<UDataTable>(NULL, DataTablePath);\
	}\
	checkf(DataType##DT, TEXT("Not Find %sDT"),#DataType);\
	F##DataType* Attr = DataType##DT->FindRow<F##DataType>(RowName, FString("None"));\
	return Attr;\
}\

DATATABLE_Impl(NavButtonAttr, TEXT("DataTable'/Game/NavButtonAttrs_DT.NavButtonAttrs_DT'"));
DATATABLE_Impl(BagWidgetType1Attr, TEXT("DataTable'/Game/BagWidgetType1Attrs_DT.BagWidgetType1Attrs_DT'"));
DATATABLE_Impl(BagGridWidgetType1Attr, TEXT("DataTable'/Game/BagGridWidgetType1Attrs_DT.BagGridWidgetType1Attrs_DT'"));
DATATABLE_Impl(ItemOnGroundAttr, TEXT("DataTable'/Game/ItemOnGroundAttrs_DT.ItemOnGroundAttrs_DT'"));
DATATABLE_Impl(ItemTipBorderAttr, TEXT("DataTable'/Game/ItemTipBorderAttr_DT.ItemTipBorderAttr_DT'"));

