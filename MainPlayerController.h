// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMERUE4HARDWOR_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// ref to umg asset in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDoverlapAsset;
	

	// hold wiget after create varible
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* HUDOverlay;
protected:
	virtual void BeginPlay() override;

	
	
};
