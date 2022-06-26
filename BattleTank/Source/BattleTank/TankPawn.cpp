// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

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

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CurrentLocation = GetActorLocation();
	FVector MovePosition;

	if (TargetYAxisValue) {
		MovePosition = CurrentLocation + GetActorForwardVector() * MoveSpeed * TargetYAxisValue;
	}		
	else {
		MovePosition = CurrentLocation + GetActorRightVector() * MoveSpeed * TargetXAxisValue;
	}

	SetActorLocation(MovePosition, true);
}
