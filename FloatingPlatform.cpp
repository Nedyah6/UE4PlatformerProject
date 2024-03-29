// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include"TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);
	InterpSpeed = 4.0f;
	InterpTime = 1.0f;
	bInterping = false;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
// endpoint needs to know start point on begin play 
	EndPoint += StartPoint;
	bInterping = false;
	GetWorldTimerManager().SetTimer(InterTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);
		float DistanceTravel = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTravel<= 1.0f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterTimer, this, &AFloatingPlatform::ToggleInterping,InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}

	}

}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}
void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	//store vec1 in temp first
	VecOne = VecTwo;
	// so we dont lose vec vec1 here
	VecTwo = Temp;

}