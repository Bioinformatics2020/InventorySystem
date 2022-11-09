// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Engine/DataTable.h>
#include "StructTypes.generated.h"

/** ����������������� */
USTRUCT(BlueprintType, Blueprintable)
struct FBagGridData
{
	GENERATED_BODY()
public:
	FBagGridData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Num;
};

/** ���������е��������� */
USTRUCT(BlueprintType, Blueprintable)
struct FBagData
{
	GENERATED_BODY()
public:
	FBagData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int GridNums;//���ߵ�����
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int> HadItems;//��ӵ�й��ĵ���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FBagGridData> GridDatas;
};

/** ��ҵ��������� */
USTRUCT(BlueprintType, Blueprintable)
struct FPlayerData
{
	GENERATED_BODY()
public:
	FPlayerData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Name;//���Name
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Level;//���ˮƽ/�ȼ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Location;//���λ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FBagData BagData;//��ұ�������
};

/** ������Ʒ���� */
USTRUCT(BlueprintType, Blueprintable)
struct FItemOnGroundData
{
	GENERATED_BODY()
public:
	FItemOnGroundData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int Num;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Location;
};

/** �������� */
USTRUCT(BlueprintType, Blueprintable)
struct FGroundData
{
	GENERATED_BODY()
public:
	FGroundData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FName, FItemOnGroundData> ItemOnGroundDatas;
};

/** ��Ϸ��������Ҫ�浵������ */
USTRUCT(BlueprintType, Blueprintable)
struct FGameSaveData
{
	GENERATED_BODY()
public:
	FGameSaveData() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FPlayerData PlayerData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FGroundData GroundData;
};

/** ��Ʒ�������� */
USTRUCT(BlueprintType, Blueprintable)
struct FItemTipBorderAttr :public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemTipBorderAttr() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Info;
};

/** ������Ʒ���� */
USTRUCT(BlueprintType, Blueprintable)
struct FItemOnGroundAttr :public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemOnGroundAttr() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName ActorType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TipBorderWidgetType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TipContextWidgetType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxStackNum;
};

/** �����ڰ�ť����1����Ϣ�ṹ�� */
USTRUCT(BlueprintType, Blueprintable)
struct FBagGridWidgetType1Attr :public FTableRowBase
{
	GENERATED_BODY()
public:
	FBagGridWidgetType1Attr() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName ItemIamgeTexture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName BorderTexture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName BagGridBGTexture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D BagGridSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int MaxNum;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TipBorderWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TipContextWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName MoveItemWidgetType;
};

/** �����ؼ�����1����Ϣ�ṹ�� */
USTRUCT(BlueprintType, Blueprintable)
struct FBagWidgetType1Attr :public FTableRowBase
{
	GENERATED_BODY()
public:
	FBagWidgetType1Attr() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TitleName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName TitleTexture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName BagBGTextue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FName> ButtonIcons;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D BagSize;
};

/** ������ť����Ϣ�ṹ�� */
USTRUCT(BlueprintType, Blueprintable)
struct FNavButtonAttr :public FTableRowBase
{
	GENERATED_BODY()
public:
	FNavButtonAttr() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName HotKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Size;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FName> ButtonIcons;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName OnClickedEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName UIType;
};

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UStructTypes : public UObject
{
	GENERATED_BODY()
	
};
