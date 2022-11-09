// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructTypes.h"
#include "FileTool.generated.h"

enum class ESaveType
{
	JSON
};


class FJsonTool
{
	friend class FFileTool;
private:
	static FGameSaveData GetGameSaveDataFormJsonStr(const FString& InJsonStr);
	static FString GetJsonStrFromGameSaveData(const FGameSaveData& InGameSaveData);
};

/** �ṩ�ļ���д���� */
class FFileTool
{
public:
	static bool SaveGameSaveDataToFile(const FGameSaveData& InGameSaveData, FString FileName = FString("Save00.ini"), FString RelativePath=FString("GameInfo/"), ESaveType SaveType = ESaveType::JSON);
	static bool LoadGameSaveDataInFile(FGameSaveData& OutGameSaveData, FString FileName = FString("Save00.ini"), FString RelativePath = FString("GameInfo/"), ESaveType SaveType = ESaveType::JSON);

private:
	/** ���·����������Ҫ��'/'��ͷ */
	static bool SaveStrToFile(const FString& InContext, FString RelativePath, FString FileName);
	static bool LoadStrInFile(FString& OutContext, FString RelativePath, FString FileName);
};

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UFileTool : public UObject
{
	GENERATED_BODY()
	
};
