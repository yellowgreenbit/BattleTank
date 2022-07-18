// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	APhysicsProjectile();
	virtual void Start() override;

protected:
	virtual void Move() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UPhysicsComponent* PhysicsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		float MovementAccurency = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		float MaxTimeSimulation = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		float TimeStep = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		float MoveSpeedPhysics = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		FVector MoveVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		TArray<FVector> CurrentTrajectory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		int TrajectoryPointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		bool bShowTrajectory = true;


	//void Explode();
};
