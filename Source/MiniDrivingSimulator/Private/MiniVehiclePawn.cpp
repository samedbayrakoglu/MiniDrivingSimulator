// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniVehiclePawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMiniVehiclePawn::AMiniVehiclePawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMiniVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (VehicleMappingContext)
                {
                    Subsystem->AddMappingContext(VehicleMappingContext, 0);
                }
            }
        }
    }
}

// Called every frame
void AMiniVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMiniVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ThrottleAction)
        {
            EnhancedInputComponent->BindAction(
                ThrottleAction,
                ETriggerEvent::Triggered,
                this,
                &AMiniVehiclePawn::OnThrottle
            );

            EnhancedInputComponent->BindAction(
                ThrottleAction,
                ETriggerEvent::Completed,
                this,
                &AMiniVehiclePawn::OnThrottle
            );
        }

        if (BrakeAction)
        {
            EnhancedInputComponent->BindAction(
                BrakeAction,
                ETriggerEvent::Triggered,
                this,
                &AMiniVehiclePawn::OnBrake
            );

            EnhancedInputComponent->BindAction(
                BrakeAction,
                ETriggerEvent::Completed,
                this,
                &AMiniVehiclePawn::OnBrake
            );
        }

        if (SteeringAction)
        {
            EnhancedInputComponent->BindAction(
                SteeringAction,
                ETriggerEvent::Triggered,
                this,
                &AMiniVehiclePawn::OnSteering
            );

            EnhancedInputComponent->BindAction(
                SteeringAction,
                ETriggerEvent::Completed,
                this,
                &AMiniVehiclePawn::OnSteering
            );
        }

        if (HandbrakeAction)
        {
            EnhancedInputComponent->BindAction(
                HandbrakeAction,
                ETriggerEvent::Started,
                this,
                &AMiniVehiclePawn::OnHandbrake
            );

            EnhancedInputComponent->BindAction(
                HandbrakeAction,
                ETriggerEvent::Completed,
                this,
                &AMiniVehiclePawn::OnHandbrake
            );
        }

		if (ToggleCameraAction)
		{
			EnhancedInputComponent->BindAction(
				ToggleCameraAction,
				ETriggerEvent::Started,
				this,
				&AMiniVehiclePawn::OnToggleCamera
			);
		}
    }
}

void AMiniVehiclePawn::OnThrottle(const FInputActionValue& Value)
{
    const float ThrottleInput = Value.Get<float>();

	// UE_LOG(LogTemp, Warning, TEXT("THROTTLE: %f"), ThrottleInput);

    GetVehicleMovementComponent()->SetThrottleInput(ThrottleInput);
}

void AMiniVehiclePawn::OnBrake(const FInputActionValue& Value)
{
    const float BrakeInput = Value.Get<float>();

    GetVehicleMovementComponent()->SetBrakeInput(BrakeInput);
}

void AMiniVehiclePawn::OnSteering(const FInputActionValue& Value)
{
    const float SteeringInput = Value.Get<float>();

    GetVehicleMovementComponent()->SetSteeringInput(SteeringInput);
}

void AMiniVehiclePawn::OnHandbrake(const FInputActionValue& Value)
{
    const bool bHandbrakePressed = Value.Get<bool>();

    GetVehicleMovementComponent()->SetHandbrakeInput(bHandbrakePressed);
}

void AMiniVehiclePawn::OnToggleCamera(const FInputActionValue& Value)
{
    UCameraComponent* BackCam = FindComponentByClass<UCameraComponent>();

    TArray<UCameraComponent*> Cameras;
    GetComponents<UCameraComponent>(Cameras);

    if (Cameras.Num() < 2)
    {
        return;
    }

    UCameraComponent* FrontCam = Cameras[1];

    bool bBackActive = BackCam->IsActive();

    BackCam->SetActive(!bBackActive);
    FrontCam->SetActive(bBackActive);
}
