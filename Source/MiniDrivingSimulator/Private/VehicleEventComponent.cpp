// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleEventComponent.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values for this component's properties
UVehicleEventComponent::UVehicleEventComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVehicleEventComponent::BeginPlay()
{
    Super::BeginPlay();

    Vehicle = Cast<AWheeledVehiclePawn>(GetOwner());

    if (Vehicle)
    {
        Movement = Cast<UChaosWheeledVehicleMovementComponent>(
            Vehicle->GetVehicleMovementComponent()
        );
    }
}


// Called every frame
void UVehicleEventComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CheckHardBraking(DeltaTime);
}


// Hard braking detection
void UVehicleEventComponent::CheckHardBraking(float DeltaTime)
{
    if (!Movement || DeltaTime <= 0.0f)
    {
        return;
    }

    const float CurrentSpeed = Movement->GetForwardSpeed();

    // Unreal: cm/s → m/s
    const float CurrentSpeedMps = CurrentSpeed / 100.0f;
    const float PreviousSpeedMps = PreviousSpeed / 100.0f;

    const float Acceleration =
        (CurrentSpeedMps - PreviousSpeedMps) / DeltaTime;

    const bool bHardBrakingNow = Acceleration < -8.0f;

    if (bHardBrakingNow && !bIsHardBraking)
    {
        const FDateTime Timestamp = FDateTime::Now();

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("[%s] HARD BRAKING EVENT! Deceleration: %.2f m/s^2"),
            *Timestamp.ToString(TEXT("%Y-%m-%d %H:%M:%S.%s")),
            Acceleration
        );
    }

    bIsHardBraking = bHardBrakingNow;
    PreviousSpeed = CurrentSpeed;
}