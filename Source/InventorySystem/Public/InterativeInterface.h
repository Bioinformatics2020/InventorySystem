// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterativeComponent.h"
#include "InterativeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterativeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInterativeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginReadyInterative(AActor* Actor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndReadyInterative(AActor* Actor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TickReadyInterative(AActor* Actor, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void BeginInterative(AActor* Actor, UInterativeComponent* InterativeComponent);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EndInterative(AActor* Actor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void TickInterative(AActor* Actor, float DeltaTime);

	
};
