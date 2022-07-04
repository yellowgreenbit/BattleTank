// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components\SceneComponent.h"
#include "Components\StaticMeshComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(SceneComponent);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);

	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel1);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, MoveRate);

}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed	* MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	OtherActor->Destroy();
	Destroy();
}
