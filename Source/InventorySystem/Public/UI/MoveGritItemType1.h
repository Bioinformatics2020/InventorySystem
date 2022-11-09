// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoveGritItemType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UMoveGritItemType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void Init(UObject* NewIamge);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* ItemImage;
};
