// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleTelemetryUdpSender.h"
#include "VehicleTelemetryComponent.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Common/UdpSocketBuilder.h"

// Sets default values for this component's properties
UVehicleTelemetryUdpSender::UVehicleTelemetryUdpSender()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVehicleTelemetryUdpSender::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// handle socket
	Socket = FUdpSocketBuilder(TEXT("VehicleTelemetrySocket"))
		.AsReusable()
		.AsNonBlocking()
		.WithBroadcast();

	TelemetryComponent = GetOwner()->FindComponentByClass<UVehicleTelemetryComponent>();

	// handle timer
	GetWorld()->GetTimerManager().SetTimer(
    SendTimerHandle,
    this,
    &UVehicleTelemetryUdpSender::SendTelemetry,
    0.01f,
    true
	);
	
}


// Called every frame
void UVehicleTelemetryUdpSender::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleTelemetryUdpSender::SendTelemetry()
{
	if (!Socket || !TelemetryComponent)
	{
		return;
	}

	const FVehicleTelemetryData& Data = TelemetryComponent->CurrentTelemetry;

	TArray<uint8> Packet;
	Packet.SetNumUninitialized(sizeof(float) * 8);

	float* Buffer = reinterpret_cast<float*>(Packet.GetData());

	Buffer[0] = Data.Speed;
	Buffer[1] = Data.RPM;
	Buffer[2] = Data.WorldPosition.X;
	Buffer[3] = Data.WorldPosition.Y;
	Buffer[4] = Data.WorldPosition.Z;
	Buffer[5] = Data.SteeringAngle;
	Buffer[6] = Data.Throttle;
	Buffer[7] = Data.Brake;

	FIPv4Address DestinationAddress(127, 0, 0, 1);
	FIPv4Endpoint Destination(DestinationAddress, 5005);

	int32 BytesSent = 0;

	Socket->SendTo(
		Packet.GetData(),
		Packet.Num(),
		BytesSent,
		*Destination.ToInternetAddr()
	);
}

