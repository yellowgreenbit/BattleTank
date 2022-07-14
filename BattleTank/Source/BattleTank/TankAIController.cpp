// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TankPawn)
	{
		Initialize();
	}

	if (!TankPawn)
	{
		return;
	}

	TankPawn->MoveForward(1);

	float RotationValue = GetRotationValue();

	TankPawn->RotateRight(RotationValue);

	Targeting();
}

float ATankAIController::GetRotationValue()
{

	FVector CurrentPoint = PatrollingPath[CurrentPatrollingIndex];
	FVector PawnLocation = TankPawn->GetActorLocation();

	if (FVector::Distance(CurrentPoint, PawnLocation) <= MovementAccurency) {
		CurrentPatrollingIndex++;
		if (CurrentPatrollingIndex >= PatrollingPath.Num()) {
			CurrentPatrollingIndex = 0;
		}
	}

	FVector MoveDirection = CurrentPoint - PawnLocation;
	MoveDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), PawnLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	float ForwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ForwardDirection, MoveDirection)));
	float RightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(RightDirection, MoveDirection)));

	float RotationValue = 0;

	if (ForwardAngle > 5) {
		RotationValue = 1;
	}

	if (ForwardAngle > 90) {
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

void ATankAIController::Targeting()
{
	if (CanFire()) {
		Fire();
	}
	else {
		RotateToPlayer();
	}
}

void ATankAIController::RotateToPlayer()
{
	if (IsPlayerRange()) {
		TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
	}
}

bool ATankAIController::IsPlayerRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}

bool ATankAIController::CanFire()
{
	if (!IsPlayerSeen()) {
		return false;
	}
	
	FVector TargetingDir = TankPawn->GetTurretForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));

	return AimAngle <= Accurency;
}

void ATankAIController::Fire()
{
	TankPawn->Fire();
}

bool ATankAIController::IsPlayerSeen()
{
	if (PlayerPawn) {
		FVector PlayerPos = PlayerPawn->GetActorLocation();
		FVector EyesPos = TankPawn->GetEyesPosition();

		FHitResult HitResult;

		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(TankPawn);
		TraceParams.bReturnPhysicalMaterial = false;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			if (HitResult.GetActor())
			{
				DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);

				return HitResult.GetActor() == PlayerPawn;
			}
		}
		DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Purple, false, 0.5f, 0, 10);
	}

	return false;
}

void ATankAIController::Initialize()
{

	TankPawn = Cast<ATankPawn>(GetPawn());

	if (!TankPawn) return;

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector PawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetAccurency();
	TArray<FVector> Points = TankPawn->GetPatrollingPoints();

	for (FVector Point : Points)
	{
		PatrollingPath.Add(Point + PawnLocation);
	}

	CurrentPatrollingIndex = 0;
}
