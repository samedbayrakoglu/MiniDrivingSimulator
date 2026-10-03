#pragma once

#include "CoreMinimal.h"
#include "VehicleTelemetryData.generated.h"

USTRUCT(BlueprintType)
struct FVehicleTelemetryData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float Speed = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RPM = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FVector WorldPosition = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    float SteeringAngle = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float Throttle = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float Brake = 0.0f;
};