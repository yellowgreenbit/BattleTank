// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UCLASS()
class BATTLETANK_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();
	
	void Fire();
	void FireSpecial();
	void Reload();
	void AddAmmo(int Value);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ECannonType CannonType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ReloadTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float ReloadSpecailAmmoTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int Ammo = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int FireSpecialAmmo = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float FireSpecialNextAmmoTime = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float RireRange = 100.f;

	FTimerHandle ReloadTimer;
	FTimerHandle ReloadFireSpecialTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

private:
	void FireSpecialQueue();
	void LaunchProjectile();
	bool bCanFire = true;
	int AmmoQueue = FireSpecialAmmo;
};
