// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATankController();

	virtual void SetupInputComponent() override;

	virtual void SetPawn(class APawn* InPawn) override;

	UPROPERTY()
		class ATankPawn* TankPawn;

	UPROPERTY()
	FVector MousePos = FVector::ZeroVector;

	virtual void Tick(float DeltaSeconds) override;

	FVector GetMousePosition() { return MousePos; }

protected:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void RotateRight(float Value);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void FireSpecial();

	UFUNCTION()
		void ChangeCannonType();

	UFUNCTION()
		void ShowTrajectory(float Value);
};
