// Fill out your copyright notice in the Description page of Project Settings.

#include "Main.h"
//#include  "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
//#include "ThirdPersonCheckCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
//#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// pull towards player if collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	
	CameraBoom->TargetArmLength = 600.0f;// camera follow at distance
	CameraBoom->bUsePawnControlRotation = true;// rotate arm based on controller
	// set size for collision capsule
	GetCapsuleComponent()->SetCapsuleSize(48.0f, 105.0f);
											   
											   // create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// dont want camera to follow along controller rotation
	// want camera to follow boom atach camera to end of boom
	// let boom ajust to match controller orientation

	FollowCamera->bUsePawnControlRotation = false;

	// set the turn rates for input
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;
	// stop chactactor changing with camera rotoate when controller rotate
	// effect camera only
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bShiftKeyDown = false;
	bLMBDown = false;
	bAttacking = false;
	// charator turns towards direction it is moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// at this rotation rate
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	MaxHealth =100.0f;
	Health = 100.0f;
	MaxStamina = 150.0f;
	Stamina = 120.0f;
	Coin= 0;
	RunningSpeed = 650.0f;
	SprintingSpeed = 900.0f;
	// enum status
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;
	StaminaDrainRate = 25.0f;
	MinSprintStamina = 50.0f;

}


// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// delta stamina makes stamina go up again after drain
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			//check if drainstamina would put us i nmin state
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{// if stamina is super low drop it int belowMinun
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{// if not less than min sprint stamina 
				Stamina -= DeltaStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}

		else
		{
			// shift key up
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
	break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			//drain unless stamina is empty 
			if (Stamina - DeltaStamina <= 0.0f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.0f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{// shift key up
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{// stamina out of low range put stamina back to normal state
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}

		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.0f;

		}
		else
		{// shift key up 
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
	
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);
	
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);
}


void AMain::MoveForward(float Value)
{
	if ((Controller != nullptr)&&(  Value != 0.0f)&& (!bAttacking))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AMain::MoveRight(float Value)
{
	
	
		if ((Controller != nullptr) && (Value != 0.0f)&&(!bAttacking))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);

		}

}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void AMain::LMBDown()
{
	bLMBDown = true;
	if (ActiveOverlapingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlapingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if(EquippedWeapon)
	{
		Attack();
	}

}
void AMain::LMBUp()
{
	bLMBDown = false;
}


void AMain::LookUpAtRate(float Rate)
{
	
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());


}
void AMain::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.0f)
	{
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coin += Amount;
}
void AMain::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("DEAD"));
}
void AMain::SetMovementStatus(EMovementStatus Status)
{// set movement status to status that is passed in 
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}
void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}
void AMain::ShowPickUpLocation()
{
	// good for looping through part of loop
	// or good for counter 
	//for (int32 i = 0; i < PickUpLocation.Num(); i++)
//	{
		//UKismetSystemLibrary::DrawDebugSphere(this, PickUpLocation[i],25.0f, 8, FLinearColor::Green, 10.5f,0.5f);
	//}

	for (FVector  Location: PickUpLocation)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8, FLinearColor::Green, 10.5f, 0.5f);
	}

	
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet;
}

void AMain::Attack()
{
	// remember attack end is called in anim BP
	if (!bAttacking)
	{
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0,1);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.24f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			default:
				
				;
			}
		
		}

	}
}
void AMain::AttackEnd()
{// attacked end is called in BP anim
	bAttacking = false;
	if (bLMBDown)
	{
		Attack();
	}
}