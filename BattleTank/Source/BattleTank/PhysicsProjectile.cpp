// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>
#include "DamageTaker.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));
}

void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * MoveSpeedPhysics;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, MaxTimeSimulation, TimeStep, 0.f);

	if (bShowTrajectory)
	{
		for (FVector Position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}

	TrajectoryPointIndex = 0;

	Super::Start();
}

void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeedPhysics * MoveRate;
	SetActorLocation(NewLocation);

	if (FVector::Distance(NewLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MovementAccurency)
	{
		TrajectoryPointIndex++;

		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			AddComplexForce();
			Destroy();			
		}
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);

			SetActorRotation(NewRotation);
		}
	}
}

/*
void APhysicsProjectile::Explode()
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

			IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
			if (DamageTakerActor)
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.DamageInstigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(DamageData);
			}
			else {
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

}
*/
