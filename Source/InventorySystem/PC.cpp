// Fill out your copyright notice in the Description page of Project Settings.


#include "PC.h"
#include <Blueprint/UserWidget.h>

void APC::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(true).SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).SetWidgetToFocus(NULL);
	SetInputMode(InputMode);
	SetShowMouseCursor(true);

	UClass* HUBWidgetClass = LoadClass<UUserWidget>(NULL, TEXT("WidgetBlueprint'/Game/UMG/HUBWidget_WBP.HUBWidget_WBP_C'"));
	UUserWidget* HUBWidget = CreateWidget(this, HUBWidgetClass);
	HUBWidget->AddToViewport();
}
