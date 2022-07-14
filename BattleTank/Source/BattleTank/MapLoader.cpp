#include "MapLoader.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PointLightComponent.h>
#include <Components/BoxComponent.h>
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"

AMapLoader::AMapLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ActivatedLight"));
	ActivatedLight->SetupAttachment(SceneComp);

	DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DeactivatedLight"));
	DeactivatedLight->SetupAttachment(SceneComp);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(BuildingMesh);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);
}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
	IsActivated = NewIsActivated;
	SetActivateLights();
}

void AMapLoader::BeginPlay()
{
	Super::BeginPlay();

	SetActivateLights();
}

void AMapLoader::SetActivateLights()
{
	ActivatedLight->SetHiddenInGame(!IsActivated);
	DeactivatedLight->SetHiddenInGame(IsActivated);
}

void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActivated) {
		return;
	}

	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn) {
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
}

