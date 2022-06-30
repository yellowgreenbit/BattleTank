// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components\StaticMeshComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"
#include "TankController.h"
#include "Cannon.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

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
	/*
	UE_LOG(LogTemp, Warning,
		TEXT("CurrentRotateAxis Value: %f, RotateRightAxisValue: %f"), 
		CurrentRotateAxisValue, RotateRightAxisValue
	);
	*/
	FRotator CurrentRotation = GetActorRotation();

	YawRotation += CurrentRotation.Yaw;

	FRotator newRotation = FRotator(0.f, YawRotation, 0.f);

	SetActorRotation(newRotation);

	// Turret rotation

	FVector MousePos = TankController->GetMousePosition();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
	FRotator TurretRotation = TurretMesh->GetComponentRotation();

	TargetRotation.Pitch = TurretRotation.Pitch;
	TargetRotation.Roll = TurretRotation.Roll;

	TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, RotateInterpolationKey));
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector TankLocation = GetActorLocation();
	
	SetActorLocation(FVector(TankLocation.X, TankLocation.Y, 5.0f));

	TankController = Cast<ATankController>(GetController());

	SetupCannon();
}

void ATankPawn::SetupCannon()
{
	if (!CannonClass) {
		return;
	}

	if (Cannon) {
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);

	Cannon->AttachToComponent(TurretMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
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
