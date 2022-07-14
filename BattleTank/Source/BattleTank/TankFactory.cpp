
#include "TankFactory.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Components/ArrowComponent.h>
#include "HealthComponent.h"
#include "TankPawn.h"
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystemComponent.h>


ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	DestroyBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DestroyBuildingMesh"));
	DestroyBuildingMesh->SetupAttachment(SceneComp);
	DestroyBuildingMesh->SetVisibility(false);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(BuildingMesh);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TankSpawnPoint"));
	TankSpawnPoint->SetupAttachment(BuildingMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);

	Spawn_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Spawn Audio"));
	Spawn_Audio->SetAutoActivate(false);

	Die_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Die Audio"));
	Die_Audio->SetAutoActivate(false);

	Spawn_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spawn Effect"));
	Spawn_Effect->SetupAttachment(SceneComp);
	Spawn_Effect->SetAutoActivate(false);
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle SpawnTimer;

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ATankFactory::SpawnTank, SpawnTankRate, true, 2.f);
}

void ATankFactory::SpawnTank()
{
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1, 1, 1));
	ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewTank->SetPatrollingPoints(TankWayPoints);

	UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);

	Spawn_Audio->Play();
	Spawn_Effect->ActivateSystem();
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankFactory::Die()
{
	BuildingMesh->SetVisibility(false);
	DestroyBuildingMesh->SetVisibility(true);
	Die_Audio->Play();
	//Destroy();
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s take Damage: %f, Health: %f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

