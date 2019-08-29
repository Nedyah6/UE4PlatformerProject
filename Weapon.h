// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_PickUp UMETA(DisplayName ="PickUp"),
	EMS_Equipped UMETA(DisplayeName ="Ëquipped"),
	EMS_MAX UMETA (DisplayeName ="DefaultMax")

};
/**
 * 
 */
UCLASS()
class PLATFORMERUE4HARDWOR_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Item ")
	EWeaponState WeaponState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	bool bWeaponParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeleetalMesh")
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item| Combat")
		
	class UBoxComponent* CombatCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Item | Combat")
	float Damge;
protected:
	virtual void BeginPlay() override;
	
public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMain* Char);

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState =State;}
	
	FORCEINLINE	EWeaponState GetWeaponState() { return WeaponState;}

	UFUNCTION()
	 void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	 void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
};
