// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	RootComponent = CannonMesh;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::SingleFire()
{

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

	if (CannonType == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire projectile. Count: %d"), Ammo));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Fire trace. Count: %d"), Ammo));
	}

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

void ACannon::Reload()
{
	bCanFire = true;
}
