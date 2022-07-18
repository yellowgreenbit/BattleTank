// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();

	bool bIsActivation = false;

	virtual void Start();

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float MoveRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float DeactivateTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	float PushForce = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components");
	bool bEnableVolumeExplode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Params")
		float ExplodeRadius = 200.0f;

	FTimerHandle MoveTimer;
	FTimerHandle DeactivateTimer;

	void AddSingleForce(AActor* OtherActor);
	void AddComplexForce();
	void Deactivate();

	virtual void Move();

};
