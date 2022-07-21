// Fill out your copyright notice in the Description page of Project Settings.


#include "TankController.h"
#include "TankPawn.h"
#include <Engine/EngineTypes.h>

ATankController::ATankController()
{
	bShowMouseCursor = true;
}

void ATankController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankController::RotateRight);
	InputComponent->BindAxis("AIM", this,  &ATankController::ShowTrajectory);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this,  &ATankController::Fire);
	InputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this,  &ATankController::FireSpecial);
	InputComponent->BindAction("ChangeCannonType", EInputEvent::IE_Pressed, this,  &ATankController::ChangeCannonType);
}

void ATankController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (TankPawn) {

		FVector TankPosition = TankPawn->GetActorLocation();
		FHitResult HitResult;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_EngineTraceChannel2, false, HitResult);

		if (HitResult.GetActor())
		{
			MousePos = HitResult.Location;
		}

	}
}

void ATankController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	TankPawn = Cast<ATankPawn>(InPawn);
}

void ATankController::MoveForward(float Value)
{
	if (TankPawn) {
		TankPawn->MoveForward(Value);
	}	
}

void ATankController::MoveRight(float Value)
{
	if (TankPawn) {
		TankPawn->MoveRight(Value);
	}	
}

void ATankController::RotateRight(float Value)
{
	if (TankPawn) {
		TankPawn->RotateRight(Value);
	}	
}

void ATankController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}
}

void ATankController::ChangeCannonType()
{
	if (TankPawn)
	{
		TankPawn->ChangeCannonType();
	}
}

void ATankController::ShowTrajectory(float Value)
{
	if (TankPawn && Value) {
		FVector ProjectileSpawnPoint = TankPawn->GetCannon()->GetProjectileSpawnPoint()->GetComponentLocation();
		//FVector TankPosition = TankPawn->GetActorLocation();
		DrawDebugLine(GetWorld(), ProjectileSpawnPoint, MousePos, FColor::Green, false, .2f, 0, 5);
	};
}
