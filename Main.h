// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum  class EStaminaStatus :uint8
{
	ESS_Normal UMETA (DisplayName ="Normal"),
	ESS_BelowMinimum UMETA (DisplayName ="BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName ="Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName= "ExhaustedRecovering"),
	ESS_MAX UMETA(DisplayName= "DefaultMax")
};
UCLASS()
class PLATFORMERUE4HARDWOR_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();
	TArray<FVector> PickUpLocation;
	UFUNCTION(BlueprintCallable)
	void ShowPickUpLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Items")
	class AItem* ActiveOverlapingItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Enums")
	EMovementStatus MovementStatus;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category ="Enums")
	EStaminaStatus StaminaStatus;
	
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlapingItem = Item; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement")
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	// set move ment status and running speed
	void SetMovementStatus(EMovementStatus Status);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Running")
	float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Running")
	float SprintingSpeed;
	bool bShiftKeyDown;

	// press down to sprint 
	void ShiftKeyDown();
	void ShiftKeyUp();
	/*
	position camera behind the player
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/*
	follow camera
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	/*
	base turn rate to scale turning functions for camera
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;

	void DecrementHealth(float Amount);
	void IncrementCoins(int32 Amount);
	void Die();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// called for forward backwards input
	void MoveForward(float Value);

	// called for side to side
	void MoveRight(float Value);

	// called input to turn at given rate
	// this is a noramlised rate 1.0 means 100% of desired turnrate
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE  class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	void  SetEquippedWeapon(AWeapon* WeaponToSet);

	FORCEINLINE  AWeapon* GetEquippedWeapon() { return EquippedWeapon ; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Anims")
	bool bAttacking;
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Anims")
	class UAnimMontage* CombatMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category ="Player Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Player Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly ,Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Player Stats")
	float Stamina;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Player Stats")
	int32 Coin;

};