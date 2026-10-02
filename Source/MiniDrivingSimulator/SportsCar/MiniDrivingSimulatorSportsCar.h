// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MiniDrivingSimulatorPawn.h"
#include "MiniDrivingSimulatorSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class AMiniDrivingSimulatorSportsCar : public AMiniDrivingSimulatorPawn
{
	GENERATED_BODY()
	
public:

	AMiniDrivingSimulatorSportsCar();
};
