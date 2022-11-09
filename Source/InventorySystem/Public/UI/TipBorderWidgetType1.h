// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TipBorderWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UTipBorderWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void Init(int ID, int Num, UWidget* ContextWidget);
	UFUNCTION()
		void UpData(int Num);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UNamedSlot* ContextSlot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* TitleName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UBorder* BGBorder;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class USizeBox* WidgetSizeBox;

	class UWidget* ChildContextWidget;
};
