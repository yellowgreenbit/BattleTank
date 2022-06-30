// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStruct.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(DisplayName = "UseProjectile"),
	FireTrace = 1 UMETA(DisplayName = "Use Trace")
};

UCLASS()
class BATTLETANK_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};
