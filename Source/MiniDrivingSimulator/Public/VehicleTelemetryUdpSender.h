// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleTelemetryUdpSender.generated.h"

class UVehicleTelemetryComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIDRIVINGSIMULATOR_API UVehicleTelemetryUdpSender : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleTelemetryUdpSender();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	TObjectPtr<UVehicleTelemetryComponent> TelemetryComponent;

	FTimerHandle SendTimerHandle;

	void SendTelemetry();

	FSocket* Socket = nullptr;

	uint32 SequenceNumber = 0;
};
