// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TipContextWidgetType1.h"
#include "Components/TextBlock.h"

void UTipContextWidgetType1::Init(FName Info, int Num)
{
	InfoText->SetText(FText::FromName(Info));
	NumText->SetText(FText::FromString(FString::FromInt(Num)));
}

void UTipContextWidgetType1::UpData(int Num)
{
	NumText->SetText(FText::FromString(FString::FromInt(Num)));
}
