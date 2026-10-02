// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniDrivingSimulatorGameMode.h"
#include "MiniDrivingSimulatorPlayerController.h"

AMiniDrivingSimulatorGameMode::AMiniDrivingSimulatorGameMode()
{
	PlayerControllerClass = AMiniDrivingSimulatorPlayerController::StaticClass();
}
