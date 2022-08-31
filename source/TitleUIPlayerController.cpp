// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleUIPlayerController.h"
#include "Blueprint/UserWidget.h"//유저위젯 인클루딩

void ATitleUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}