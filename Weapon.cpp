// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include"Components/SkeletalMeshComponent.h"
#include "Main.h"
#include"Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include"Kismet/GameplayStatics.h"
#include"Particles/ParticleSystemComponent.h"
#include"Components/BoxComponent.h"
#include"Enemy.h"
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
{
	SkeletalMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatColision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	bWeaponParticles = false;
	WeaponState = EWeaponState::EWS_PickUp;
	Damge = 25.0f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((WeaponState == EWeaponState::EWS_PickUp) && OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			Main->SetActiveOverlappingItem(this);
		
		}

	}

}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AMain* Main = Cast<AMain>(OtherActor);
	if (Main)
	{
		Main->SetActiveOverlappingItem(nullptr);

	}
}

void AWeapon::Equip(AMain* Char)
{
	if (Char)
	{
		// ignore camera and player collsion
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);
		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;
		
			Char->SetEquippedWeapon(this);

			Char->SetActiveOverlappingItem(nullptr);
			
		}
		if (OnEquipSound)
		{
			UGameplayStatics::PlaySound2D(this, OnEquipSound);
		}
		if (!bWeaponParticles)
		{
			IdleParticleComponent->Deactivate();
		}
	}
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (Enemy->HitParticles)
			{
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
				if (WeaponSocket)
				{
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.0f), false);
				 }

			}
		}
		
	}

}
void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}
void AWeapon::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}