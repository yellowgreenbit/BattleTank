// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "DamageTaker.h"

AMachinePawn::AMachinePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &AMachinePawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMachinePawn::DamageTaked);

	Hit_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	Hit_Effect->SetupAttachment(BodyMesh);

	Hit_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudio"));

	DieEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DieEffect"));
	DieEffect->SetupAttachment(BodyMesh);

	DieAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudio"));
}

void AMachinePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();	
}


void AMachinePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMachinePawn::Die()
{
	if (DieEffect)
		DieEffect->ActivateSystem();

	if (DieAudio)
		DieAudio->Play();

	Destroy();
}

void AMachinePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s take Damage: %f, Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
	
	if(Hit_Effect)
		Hit_Effect->ActivateSystem();

	if (Hit_Audio)
		Hit_Audio->Play();
}

