// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MoveGritItemType1.h"
#include "Components/Image.h"

void UMoveGritItemType1::Init(UObject* NewIamge)
{
	ItemImage->Brush.SetResourceObject(NewIamge);
}
