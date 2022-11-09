// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StructTypes.h"
#include "NavButtonWidgetType1.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UNavButtonWidgetType1 : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void Init(FNavButtonAttr NavButtonAttr);
	UFUNCTION()
		void OnChickedFunc();

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* NavButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class USizeBox* ButtonSizeBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* HotKeyText;
		
	FName ChickEventName;
};
