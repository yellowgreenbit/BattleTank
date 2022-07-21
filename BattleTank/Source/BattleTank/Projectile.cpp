// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DamageTaker.h"
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
	GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &AProjectile::Deactivate, DeactivateTime, false);
}

void AProjectile::AddSingleForce(AActor* OtherActor)
{
	UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

	if (Mesh)
	{
		if (Mesh->IsSimulatingPhysics()) {
			FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
			ForceVector.Normalize();
			
			Mesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
		}
	}
}

void AProjectile::AddComplexForce()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;

	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";

	TArray<FHitResult> AttachHit;

	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel(AttachHit, StartPos, EndPos,
		Rotation, ECollisionChannel::ECC_Visibility, Shape, Params);

	DrawDebugSphere(GetWorld(), StartPos, ExplodeRadius, 5, FColor::Green, false, 2.0f);

	if (bSweepResult)
	{
		for (FHitResult HitResult : AttachHit)
		{
			AActor* OtherActor = HitResult.GetActor();

			if (!OtherActor)
				continue;

			UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());

			if (Mesh)
			{
				if (Mesh->IsSimulatingPhysics()) {
					FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
					ForceVector.Normalize();

					Mesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
				}
			}
			
		}
	}
}

void AProjectile::Deactivate()
{
	bIsActivation = false;
	SetActorLocation(FVector(.0f, .0f, .0f));
	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimer);
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	SetActorEnableCollision(false);
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed	* MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	AActor* owner = GetOwner();
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;

	if (OtherActor != owner && OtherActor != OwnerByOwner)
	{
		IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);

		if (DamageTakerActor)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.DamageInstigator = owner;
			DamageData.DamageMaker = this;

			DamageTakerActor->TakeDamage(DamageData);
		}
		else {
			if (bEnableVolumeExplode) {
				AddComplexForce();
			}
			else
			{
				AddSingleForce(OtherActor);
			}
		}
	}
	
	//Destroy();
	Deactivate();
}
