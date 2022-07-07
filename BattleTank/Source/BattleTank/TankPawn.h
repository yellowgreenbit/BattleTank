// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "MachinePawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class BATTLETANK_API ATankPawn : public AMachinePawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);
	void SetupCannon(TSubclassOf<ACannon> NewCannonClass, int CountAmmo);
	void ChangeCannonType();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	void Fire();
	void FireSpecial();

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret | Component")
	TArray<TSubclassOf<ACannon>> CannonClasses;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Turret | Component")
	ACannon* Cannon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InterpolationKey = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotateInterpolationKey = 0.1f;

	UPROPERTY()
	class ATankController* TankController;

private:
	float TargetYAxisValue = 0.f;
	float TargetXAxisValue = 0.f;
	float RotateRightAxisValue = 0.f;
	float CurrentRotateAxisValue = 0.f;
};
