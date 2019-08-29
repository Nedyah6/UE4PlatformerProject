// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class PLATFORMERUE4HARDWOR_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	// overlap trigger box for player to trigger
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category ="Floor Switch")
	class  UBoxComponent* TriggerBox;
	//switch for character to step on
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Floor Switch")
	class UStaticMeshComponent* FloorSwitch;
	
	// move when floor switch is stepped on
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Floor Switch")
	UStaticMeshComponent* Door;
	// the start location for door
	UPROPERTY(BluePrintReadWrite, Category = "Floor Switch")
	FVector InitialDoorLocation;

	// start location for floor switch
	UPROPERTY(BluePrintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;

	FTimerHandle SwitchHandle;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Floor Switch")
	float SwitchTime;
	bool bCharacterOnSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void OnOverLapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Swithc")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Swithc")
	void LowerDoor();


	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Swithc")
	void RaiseFloorSwitch();


	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Swithc")
	void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "Floor Swithc")
	void UpdateDoorLocation(float Z);
	
	UFUNCTION(BlueprintCallable, Category = "Floor Swithc")
	void UpdateFloorSwitchLocation(float Z);

	void CloseDoor();
};
