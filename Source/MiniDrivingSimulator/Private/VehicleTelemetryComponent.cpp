// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleTelemetryComponent.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values for this component's properties
UVehicleTelemetryComponent::UVehicleTelemetryComponent()
{

}


// Called when the game starts
void UVehicleTelemetryComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...

	Vehicle = Cast<AWheeledVehiclePawn>(GetOwner());

	if (Vehicle)
    {
        Movement = Cast<UChaosWheeledVehicleMovementComponent>(
            Vehicle->GetVehicleMovementComponent()
        );
    }

	// handle timer
	GetWorld()->GetTimerManager().SetTimer(
    TelemetryTimerHandle,
    this,
    &UVehicleTelemetryComponent::SampleTelemetry,
    0.01f,
    true
	);
	
}

void UVehicleTelemetryComponent::SampleTelemetry()
{
	if (!Vehicle)
	{
		return;
	}

	if (!Movement)
	{
		return;
	}

	CurrentTelemetry.Speed =
		Movement->GetForwardSpeed();

	CurrentTelemetry.RPM =
    	Movement->GetEngineRotationSpeed();
	
	CurrentTelemetry.WorldPosition =
    	Vehicle->GetActorLocation();

	CurrentTelemetry.SteeringAngle =
    	Movement->GetSteeringInput();
	
	CurrentTelemetry.Throttle =
    	Movement->GetThrottleInput();
	
	CurrentTelemetry.Brake =
    	Movement->GetBrakeInput();
	
	UE_LOG(LogTemp, Warning, TEXT("Telemetry Speed: %f"), CurrentTelemetry.Speed);
	UE_LOG(LogTemp, Warning, TEXT("Telemetry rpm: %f"), CurrentTelemetry.RPM);
	UE_LOG(LogTemp, Warning, TEXT("Telemetry Pos: X=%f Y=%f Z=%f"), CurrentTelemetry.WorldPosition.X,CurrentTelemetry.WorldPosition.Y,CurrentTelemetry.WorldPosition.Z);
	UE_LOG(
    LogTemp,
    Warning,
    TEXT("Telemetry Steering: %f"),
    CurrentTelemetry.SteeringAngle
	);
	UE_LOG(
    LogTemp,
    Warning,
    TEXT("Telemetry Throttle: %f"),
    CurrentTelemetry.Throttle
	);
	UE_LOG(
    LogTemp,
    Warning,
    TEXT("Telemetry Brake: %f"),
    CurrentTelemetry.Brake
	);
}

