// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity params");
	float Gravity = -9.8f;

	TArray<FVector> GenerateTrajectory(
		FVector StartPos, 
		FVector EndPos, 
		FVector Velocity, 
		float MaxTime, 
		float TimeStep, 
		float MinZValue = 0.f
	);
};
