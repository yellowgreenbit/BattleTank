// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"
#include "Projectile.h"

void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

void AProjectilePool::GetProjectile(FVector SpawnLocation, FRotator SpawnRotator)
{
	bHaveActiveProjectile = false;

	for (int32 i = 0; i < ProjectilePool.Num(); i++)
	{
		if (ProjectilePool[i]->bIsActivation == false)
		{
			ProjectilePool[i]->SetActorLocation(SpawnLocation);
			ProjectilePool[i]->SetActorRotation(SpawnRotator);
			ProjectilePool[i]->bIsActivation = true;
			ProjectilePool[i]->SetActorEnableCollision(true);
			ProjectilePool[i]->Start();
			bHaveActiveProjectile = true;
			return;
		}
	}

	if (bHaveActiveProjectile == false)
	{
		SpawnProjectile(SpawnLocation, SpawnRotator, true);
	}
}

void AProjectilePool::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotator, bool bIsActive)
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotator);

	if (Projectile)
	{
		ProjectilePool.Add(Projectile);
		Projectile->bIsActivation = bIsActive;
		Projectile->SetActorEnableCollision(bIsActive);

		if (bIsActive)
		{
			Projectile->Start();
		}
	}
}

void AProjectilePool::InitializePool()
{
	for (int32 i = 0; i < PoolSize; i++)
	{
		SpawnProjectile(FVector(.0f, .0f, .0f), FRotator(.0f, .0f, .0f));
	}
}


