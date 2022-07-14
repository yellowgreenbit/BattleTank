// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "TankFactory.generated.h"

UCLASS()
class BATTLETANK_API ATankFactory : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	ATankFactory();

	UFUNCTION()
		virtual void TakeDamage(FDamageData DamageData) override;

protected:
	virtual void BeginPlay() override;

	void SpawnTank();

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* DestroyBuildingMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollider;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* TankSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
		TSubclassOf<class ATankPawn> SpawnTankClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	float SpawnTankRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn params")
	TArray<class ATargetPoint*> TankWayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* Spawn_Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* Spawn_Audio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UAudioComponent* Die_Audio;
};
