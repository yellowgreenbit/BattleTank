// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Projectile.h"
#include "Components\StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components\ArrowComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* CannonSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CannonRoot"));
	RootComponent = CannonSceneComponent;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(CannonSceneComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonSceneComponent);
}

void ACannon::Fire()
{
	if (!Ammo) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver, FString::Printf(TEXT("Empty bullets")));
		return;
	}

	if (!bCanFire) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Reloading...")));
		return;
	}

	Ammo--;

	LaunchProjectile();

	bCanFire = false;	

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, ReloadTime, false);
}


void ACannon::FireSpecial()
{
	if (!Ammo) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver, FString::Printf(TEXT("Empty bullets")));
		return;
	}

	if (!bCanFire) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Reloading...")));
		return;
	}

	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(
		ReloadFireSpecialTimer, 
		this,
		&ACannon::FireSpecialQueue,
		FireSpecialNextAmmoTime,
		true
	);
}

void ACannon::FireSpecialQueue() {
	if (AmmoQueue) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire special start.")));
		AmmoQueue--;
		LaunchProjectile();
	}
	else
	{
		Ammo--;
		UE_LOG(LogTemp, Warning, TEXT("ReloadFireSpecialTimer"));
		GetWorld()->GetTimerManager().ClearTimer(ReloadFireSpecialTimer);
		AmmoQueue = FireSpecialAmmo;
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire special. Count: %d"), Ammo));
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, ReloadSpecailAmmoTime, false);
	}

}

void ACannon::LaunchProjectile()
{
	if (CannonType == ECannonType::FireTrace) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire trace. Count: %d"), Ammo));

		FHitResult HitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTarce")), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;

		FVector StartTrace = ProjectileSpawnPoint->GetComponentLocation();
		FVector EndTrace = StartTrace + ProjectileSpawnPoint->GetForwardVector() * RireRange;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), StartTrace, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (HitResult.GetActor())
			{
				HitResult.GetActor()->Destroy();
			}
		}
		else {
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Orange, false, 0.5f, 0, 5);
		};
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire projectile. Count: %d"), Ammo));
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>
			(
				ProjectileClass,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation()
				);

		if (projectile) {
			projectile->Start();
		}
	}
}

void ACannon::Reload()
{
	bCanFire = true;
}

void ACannon::AddAmmo(int Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Add Ammo. ( +: %d )"), Value));
	Ammo += Value;
}
