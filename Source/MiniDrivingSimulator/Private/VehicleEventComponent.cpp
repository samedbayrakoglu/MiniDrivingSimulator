// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleEventComponent.h"
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Components/PrimitiveComponent.h"

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

        VehicleMesh = Vehicle->GetMesh();

        if (VehicleMesh)
        {
            VehicleMesh->SetNotifyRigidBodyCollision(true);

            VehicleMesh->OnComponentHit.AddDynamic(
                this,
                &UVehicleEventComponent::OnVehicleHit
            );
        }
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

    const bool bBrakeInput =
        Movement->GetBrakeInput() > 0.1f ||
        Movement->GetHandbrakeInput() > 0.1f;

    // Only detect hard braking while moving forward.
    const bool bMovingForward = CurrentSpeed > 50.0f;

    const bool bHardBrakingNow =
        bMovingForward &&
        bBrakeInput &&
        Acceleration < -8.0f;

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


// Collision detection
void UVehicleEventComponent::OnVehicleHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == GetOwner())
    {
        return;
    }

    const float ImpactStrength = NormalImpulse.Size();

    // Start a short cooldown window for this collision.
    if (!GetWorld()->GetTimerManager().IsTimerActive(CollisionTimerHandle))
    {
        HighestCollisionImpact = ImpactStrength;

        GetWorld()->GetTimerManager().SetTimer(
            CollisionTimerHandle,
            this,
            &UVehicleEventComponent::LogCollisionEvent,
            0.5f,
            false
        );
    }
    else
    {
        HighestCollisionImpact = FMath::Max(
            HighestCollisionImpact,
            ImpactStrength
        );
    }
}


// Log the strongest impact after the collision window
void UVehicleEventComponent::LogCollisionEvent()
{
    const FDateTime Timestamp = FDateTime::Now();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("[%s] COLLISION EVENT! Highest Impact Strength: %.2f"),
        *Timestamp.ToString(TEXT("%Y-%m-%d %H:%M:%S.%s")),
        HighestCollisionImpact
    );

    HighestCollisionImpact = 0.0f;
}