// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TipContextWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UTipContextWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void Init(FName Info, int Num);
	UFUNCTION()
		void UpData(int Num);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* InfoText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NumText;
};
