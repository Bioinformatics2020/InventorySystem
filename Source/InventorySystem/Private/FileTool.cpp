// Fill out your copyright notice in the Description page of Project Settings.


#include "FileTool.h"
#include <Misc/Paths.h>
#include <Serialization/JsonSerializer.h>
#include <Serialization/JsonWriter.h>

FGameSaveData FJsonTool::GetGameSaveDataFormJsonStr(const FString& InJsonStr)
{
	FGameSaveData GameSaveData;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(InJsonStr);
	TSharedPtr<FJsonValue> JsonValue;
	if (FJsonSerializer::Deserialize(JsonReader,JsonValue))
	{
		TSharedPtr<FJsonObject> GameSaveDataJsonObject = JsonValue->AsObject();

		TSharedPtr<FJsonObject> PlayerDataJsonObject = GameSaveDataJsonObject->GetObjectField(FString("PlayerData"));
		{//读取PlayerData内的数据
			FPlayerData& PlayerData = GameSaveData.PlayerData;
			PlayerData.Name = PlayerDataJsonObject->GetStringField(FString("Name"));
			PlayerData.Level = PlayerDataJsonObject->GetIntegerField(FString("Level"));
			TArray<TSharedPtr<FJsonValue>> LocationArrValue = PlayerDataJsonObject->GetArrayField(FString("Location"));
			PlayerData.Location.X = LocationArrValue[0]->AsNumber();
			PlayerData.Location.Y = LocationArrValue[1]->AsNumber();
			PlayerData.Location.Z = LocationArrValue[2]->AsNumber();
			TSharedPtr<FJsonObject> BagDataJsonObject = PlayerDataJsonObject->GetObjectField(FString("BagData"));
			
			{//读取BagData内的数据
				FBagData& BagData = PlayerData.BagData;
				BagData.GridNums = BagDataJsonObject->GetIntegerField(FString("GridNums"));

				TArray<TSharedPtr<FJsonValue>> HadItemsValue = BagDataJsonObject->GetArrayField(FString("HadItems"));
				for (TSharedPtr<FJsonValue> HadItemValue:HadItemsValue)
				{
					BagData.HadItems.Add(HadItemValue->AsNumber());
				}

				TArray<TSharedPtr<FJsonValue>> GridDatasValue = BagDataJsonObject->GetArrayField(FString("GridDatas"));
				for (TSharedPtr<FJsonValue> GridDataValue:GridDatasValue)
				{//读取BagGridData内的数据
					TSharedPtr<FJsonObject> GridDataObject = GridDataValue->AsObject();
					FBagGridData TempBagGridData;
					TempBagGridData.ID = GridDataObject->GetIntegerField(FString("ID"));
					TempBagGridData.Num = GridDataObject->GetIntegerField(FString("Num"));
					BagData.GridDatas.Add(TempBagGridData);
				}
			}
		}

		TSharedPtr<FJsonObject> GroundDataJsonObject = GameSaveDataJsonObject->GetObjectField(FString("GroundData"));
		{
			TArray<TSharedPtr<FJsonValue>> ItemOnGroundDatasArrValue = GroundDataJsonObject->GetArrayField(FString("ItemOnGroundDatas"));
			for (TSharedPtr<FJsonValue> ItemOnGroundDataValue : ItemOnGroundDatasArrValue)
			{//读取ItemOnGroundDatas内的数据
				TSharedPtr<FJsonObject> ItemOnGroundDataObject = ItemOnGroundDataValue->AsObject();
				FItemOnGroundData ItemOnGroundData;
				ItemOnGroundData.ID = ItemOnGroundDataObject->GetIntegerField(FString("ID"));
				ItemOnGroundData.Num = ItemOnGroundDataObject->GetIntegerField(FString("Num"));

				TArray<TSharedPtr<FJsonValue>> LocationArrValue = ItemOnGroundDataObject->GetArrayField(FString("Location"));
				ItemOnGroundData.Location.X = LocationArrValue[0]->AsNumber();
				ItemOnGroundData.Location.Y = LocationArrValue[1]->AsNumber();
				ItemOnGroundData.Location.Z = LocationArrValue[2]->AsNumber();

				GameSaveData.GroundData.ItemOnGroundDatas.Add(
					FName(*ItemOnGroundDataObject->GetStringField(FString("Name"))),
					ItemOnGroundData
				);
			}
		}

	}

	return GameSaveData;
}

FString FJsonTool::GetJsonStrFromGameSaveData(const FGameSaveData& InGameSaveData)
{
	FString OutJsonStr;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWrite = TJsonWriterFactory<>::Create(&OutJsonStr);
	JsonWrite->WriteObjectStart();
	{
		JsonWrite->WriteIdentifierPrefix(FString("PlayerData"));
		const FPlayerData& PlayerData = InGameSaveData.PlayerData;
		JsonWrite->WriteObjectStart();
		{//写入PlayerData
			JsonWrite->WriteValue<FString>(FString("Name"), PlayerData.Name);
			JsonWrite->WriteValue<int>(FString("Level"), PlayerData.Level);

			{//写入Location
				JsonWrite->WriteIdentifierPrefix(FString("Location"));
				JsonWrite->WriteArrayStart();
				JsonWrite->WriteValue<float>(PlayerData.Location.X);
				JsonWrite->WriteValue<float>(PlayerData.Location.Y);
				JsonWrite->WriteValue<float>(PlayerData.Location.Z);
				JsonWrite->WriteArrayEnd();
			}
			
			JsonWrite->WriteIdentifierPrefix(FString("BagData"));
			const FBagData& BagData = PlayerData.BagData;
			JsonWrite->WriteObjectStart();
			{//写入BagData
				JsonWrite->WriteValue<int>(FString("GridNums"), BagData.GridNums);

				JsonWrite->WriteIdentifierPrefix(FString("HadItems"));
				JsonWrite->WriteArrayStart();
				for (int i = 0; i < BagData.HadItems.Num(); ++i)
				{
					JsonWrite->WriteValue<int>(BagData.HadItems[i]);
				}
				JsonWrite->WriteArrayEnd();

				JsonWrite->WriteIdentifierPrefix(FString("GridDatas"));
				JsonWrite->WriteArrayStart();
				for (int i = 0; i < BagData.GridDatas.Num(); ++i)
				{
					const FBagGridData& BagGridData = BagData.GridDatas[i];
					JsonWrite->WriteObjectStart();
					{//写入GridDatas
						JsonWrite->WriteValue<int>(FString("ID"), BagGridData.ID);
						JsonWrite->WriteValue<int>(FString("Num"), BagGridData.Num);
					}
					JsonWrite->WriteObjectEnd();
				}
				JsonWrite->WriteArrayEnd();
			}
			JsonWrite->WriteObjectEnd();
		}
		JsonWrite->WriteObjectEnd();

		JsonWrite->WriteIdentifierPrefix(FString("GroundData"));
		const FGroundData& GroundData = InGameSaveData.GroundData;
		JsonWrite->WriteObjectStart();
		{
			JsonWrite->WriteIdentifierPrefix(FString("ItemOnGroundDatas"));
			JsonWrite->WriteArrayStart();
			for (TPair<FName, FItemOnGroundData>ItemOnGroundData: InGameSaveData.GroundData.ItemOnGroundDatas)
			{//写入地面物品ItemOnGroundDatas
				JsonWrite->WriteObjectStart();
				{
					JsonWrite->WriteValue<FString>(FString("Name"), ItemOnGroundData.Key.ToString());
					JsonWrite->WriteValue<int>(FString("ID"), ItemOnGroundData.Value.ID);
					JsonWrite->WriteValue<int>(FString("Num"), ItemOnGroundData.Value.Num);
					
					{//写入Location
						JsonWrite->WriteIdentifierPrefix(FString("Location"));
						JsonWrite->WriteArrayStart();
						JsonWrite->WriteValue<float>(ItemOnGroundData.Value.Location.X);
						JsonWrite->WriteValue<float>(ItemOnGroundData.Value.Location.Y);
						JsonWrite->WriteValue<float>(ItemOnGroundData.Value.Location.Z);
						JsonWrite->WriteArrayEnd();
					}
				}
				JsonWrite->WriteObjectEnd();
			}
			JsonWrite->WriteArrayEnd();
		}
		JsonWrite->WriteObjectEnd();
	}
	JsonWrite->WriteObjectEnd();
	JsonWrite->Close();
	return OutJsonStr;
}

bool FFileTool::SaveGameSaveDataToFile(const FGameSaveData &InGameSaveData, FString FileName /*= FString("Save00.ini")*/, FString RelativePath/*=FString("/GameInfo/")*/, ESaveType SaveType /*= ESaveType::JSON*/)
{
	FString DataContext;
	switch (SaveType)
	{
	case ESaveType::JSON:
		DataContext = FJsonTool::GetJsonStrFromGameSaveData(InGameSaveData);
		break;
	default:
		break;
	}

	return SaveStrToFile(DataContext, RelativePath, FileName);
}

bool FFileTool::LoadGameSaveDataInFile(FGameSaveData &OutGameSaveData, FString FileName /*= FString("Save00.ini")*/, FString RelativePath /*= FString("/GameInfo/")*/, ESaveType SaveType /*= ESaveType::JSON*/)
{
	FString DataContext;
	if (!LoadStrInFile(DataContext, RelativePath, FileName))
		return false;

	switch (SaveType)
	{
	case ESaveType::JSON:
		OutGameSaveData = FJsonTool::GetGameSaveDataFormJsonStr(DataContext);
		break;
	default:
		break;
	}
	return true;
}

bool FFileTool::SaveStrToFile(const FString& InContext, FString RelativePath, FString FileName)
{
	if (InContext.IsEmpty() || FileName.IsEmpty())
	{
		return false;
	}

	//获取完整文件名
	FString AbsPath = FPaths::ProjectSavedDir() + RelativePath;
	if (!RelativePath.IsEmpty() && RelativePath[RelativePath.Len() - 1] != TCHAR('/'))
	{
		AbsPath += '/';
	}
	AbsPath += FileName;

	//保存文件
	return FFileHelper::SaveStringToFile(InContext, *AbsPath);
}

bool FFileTool::LoadStrInFile(FString& OutContext, FString RelativePath, FString FileName)
{
	check(!FileName.IsEmpty());
	//获取完整文件名
	FString AbsPath = FPaths::ProjectSavedDir() + RelativePath;
	if (!RelativePath.IsEmpty() && RelativePath[RelativePath.Len() - 1] != TCHAR('/'))
	{
		AbsPath += '/';
	}
	AbsPath += FileName;

	//打开存档文件或初始存档
	if (FPaths::FileExists(AbsPath))
	{
		return FFileHelper::LoadFileToString(OutContext, *AbsPath);
	}
	FString InitAbsPath = FPaths::ProjectSavedDir() + FString("/GameInfo/Initial.ini");
	if (FPaths::FileExists(InitAbsPath))
	{
		return FFileHelper::LoadFileToString(OutContext, *(FPaths::ProjectSavedDir() + FString("/GameInfo/Initial.ini")));
	}
	return false;
}
