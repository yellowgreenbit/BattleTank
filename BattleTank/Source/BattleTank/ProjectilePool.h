// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "ProjectilePool.generated.h"

UCLASS()
class BATTLETANK_API AProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	void GetProjectile(FVector SpawnLocator, FRotator SpawnRotator);

protected:
	virtual void BeginPlay() override;

	void SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotator, bool bIsActive = false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
		int PoolSize = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
		TSubclassOf<AProjectile> ProjectileClass;

	TArray<AProjectile*> ProjectilePool;


private:	
	void InitializePool();

	bool bHaveActiveProjectile = false;

};
