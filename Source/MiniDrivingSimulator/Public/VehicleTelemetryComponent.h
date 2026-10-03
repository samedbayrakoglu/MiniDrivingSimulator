// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleTelemetryData.h"
#include "VehicleTelemetryComponent.generated.h"

class AWheeledVehiclePawn;
class UChaosWheeledVehicleMovementComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIDRIVINGSIMULATOR_API UVehicleTelemetryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FVehicleTelemetryData CurrentTelemetry;

	// Sets default values for this component's properties
	UVehicleTelemetryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

private:
	TObjectPtr<AWheeledVehiclePawn> Vehicle;

	TObjectPtr<UChaosWheeledVehicleMovementComponent> Movement;

	void SampleTelemetry();
};