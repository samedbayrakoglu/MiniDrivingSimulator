// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniDrivingSimulatorWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UMiniDrivingSimulatorWheelFront::UMiniDrivingSimulatorWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}