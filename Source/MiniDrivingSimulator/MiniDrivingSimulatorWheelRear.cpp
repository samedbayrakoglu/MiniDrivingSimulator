// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniDrivingSimulatorWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UMiniDrivingSimulatorWheelRear::UMiniDrivingSimulatorWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}