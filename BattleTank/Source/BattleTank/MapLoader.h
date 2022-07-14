#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

class UPointLightComponent;

UCLASS()
class BATTLETANK_API AMapLoader : public AActor
{
	GENERATED_BODY()
	
public:
	AMapLoader();

	void SetIsActivated(bool NewIsActivated);

protected:
	virtual void BeginPlay() override;

	void SetActivateLights();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPointLightComponent* ActivatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPointLightComponent* DeactivatedLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		FName LoadLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		bool IsActivated = false;

	UFUNCTION()
		void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
