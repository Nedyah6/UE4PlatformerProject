// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include"Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (HUDoverlapAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDoverlapAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}


