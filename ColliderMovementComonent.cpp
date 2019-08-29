// Fill out your copyright notice in the Description page of Project Settings.

#include "ColliderMovementComonent.h"

void UColliderMovementComonent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// check to make sure still valid and we are aloud to move
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{

		return;
	}
	// Get and clear teh vector from Collider
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f)* DeltaTime* 150.0f;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		// if we bump into something slide along the side of it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
		}

	}

}

