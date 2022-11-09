// Fill out your copyright notice in the Description page of Project Settings.


#include "EventManager.h"

TSharedPtr<FEventManager> FEventManager::Instance = nullptr;

void FEventManager::RegistEvent(FName EventName, UObject * Obj, FName FunName)
{
	if (!Get()->Events.Contains(EventName)) {
		TArray<FScriptDelegate> Delegates;
		FScriptDelegate Dele;
		Dele.BindUFunction(Obj, FunName);
		Delegates.Add(Dele);
		Get()->Events.Add(EventName, Delegates);
	}
	else
	{
		FScriptDelegate Dele;
		Dele.BindUFunction(Obj, FunName);
		Get()->Events.Find(EventName)->Add(Dele);
	}

}

void FEventManager::UnRegistEvent(FName EventName, UObject * Obj)
{
	if (Get()->Events.Contains(EventName))
	{
		TArray<FScriptDelegate> *ScriptDelegates = Get()->Events.Find(EventName);
		for (int i=0;i< ScriptDelegates->Num();i++)
		{
			if ((*ScriptDelegates)[i].GetUObject() == Obj) {
				(*ScriptDelegates)[i].Unbind();
				Get()->Events.Find(EventName)->RemoveAt(i);
				i--;
			}
		} 

		if (Get()->Events.Find(EventName)->Num() == 0)
		{
			Get()->Events.Remove(EventName);
		}
	}

	
}

void FEventManager::TriggerEvent(FName EventName, void * Parameters)
{
	if (Get()->Events.Contains(EventName)) {
		TArray<FScriptDelegate> ScriptDelegates = *Get()->Events.Find(EventName);
		for (FScriptDelegate dele :ScriptDelegates)
		{
			if (dele.GetUObject())
			{
				dele.ProcessDelegate<UObject>(Parameters);
			}
		}
	}
}
