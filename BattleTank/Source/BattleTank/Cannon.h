// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "GameFramework/Actor.h"
#include "ProjectilePool.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Cannon.generated.h"

UCLASS()
class BATTLETANK_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();
	
	bool IsReadyToFire() {
		return bCanFire;
	};

	void Fire();
	void FireSpecial();
	void Reload();
	void AddAmmo(int Value);

	void CreateProjectilePool();

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

	UPROPERTY()
	AProjectilePool* ProjectilePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<AProjectilePool> ProjectilePoolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* ShootAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		UForceFeedbackEffect* ShootForceEffect;

	virtual void BeginPlay() override;

private:
	void FireSpecialQueue();
	void LaunchProjectile();
	bool bCanFire = true;
	int AmmoQueue = FireSpecialAmmo;
};
