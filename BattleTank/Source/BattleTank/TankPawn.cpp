// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "TankController.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Engine/TargetPoint.h>

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BodyMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float Value)
{
	TargetYAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	TargetXAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Tank movement

	FVector CurrentLocation = GetActorLocation();
	FVector MovePosition;

	if (TargetYAxisValue) {
		MovePosition = CurrentLocation + GetActorForwardVector() * MoveSpeed * TargetYAxisValue;
	}		
	else {
		MovePosition = CurrentLocation + GetActorRightVector() * MoveSpeed * TargetXAxisValue;
	}
	SetActorLocation(MovePosition, true);

	// Tank rotation
	CurrentRotateAxisValue = FMath::Lerp(CurrentRotateAxisValue, RotateRightAxisValue, InterpolationKey);
	float YawRotation = RotationSpeed * CurrentRotateAxisValue * DeltaSeconds;

	FRotator CurrentRotation = GetActorRotation();

	YawRotation += CurrentRotation.Yaw;

	FRotator newRotation = FRotator(0.f, YawRotation, 0.f);

	SetActorRotation(newRotation);

	// Turret rotation
	if (TankController) {
		FVector MousePos = TankController->GetMousePosition();
		RotateTurretTo(MousePos);
	}
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector TankLocation = GetActorLocation();
	
	SetActorLocation(FVector(TankLocation.X, TankLocation.Y, 50.0f));

	TankController = Cast<ATankController>(GetController());

	SetupCannon(CannonClass, 0);
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> NewCannonClass, int CountAmmo)
{
	if (!NewCannonClass) {
		return;
	}

	bool bCannonExists = CannonClasses.Find(NewCannonClass) != -1;

	// при инициализации или
	// если пушка новая - добавляем ее переключаемся
	// если пушка не текущая - удаляем старую
	if (!bCannonExists || CannonClass != NewCannonClass || !Cannon) {
		if (Cannon) {
			Cannon->Destroy();
		}

		FActorSpawnParameters params;
		params.Instigator = this;
		params.Owner = this;

		CannonClass = NewCannonClass;

		Cannon = GetWorld()->SpawnActor<ACannon>(NewCannonClass, params);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
		
		if (!bCannonExists)
		{
			CannonClasses.Add(*CannonClass);
		}
		else if (CountAmmo) {
			Cannon->AddAmmo(CountAmmo);
		}
			
	}
	else {
		// иначе просто добавляем патроны
		if (Cannon && CountAmmo) {
			Cannon->AddAmmo(CountAmmo);
		}
	}
}

void ATankPawn::ChangeCannonType()
{
	if (Cannon)
	{
		int CurrentIndexCannon = CannonClasses.Find(CannonClass);

		if (CurrentIndexCannon == -1 || (CurrentIndexCannon == CannonClasses.Num() - 1)) {
			SetupCannon(CannonClasses[0], 0);
		}
		else {
			SetupCannon(CannonClasses[CurrentIndexCannon + 1], 0);
		}
	}
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
	TArray<FVector> points;

	for (ATargetPoint* point : PatrollingPoints)
	{
		points.Add(point->GetActorLocation());
	}

	return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();

	TargetRotation.Pitch = TurretRotation.Pitch;
	TargetRotation.Roll = TurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, RotateInterpolationKey));	
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}
