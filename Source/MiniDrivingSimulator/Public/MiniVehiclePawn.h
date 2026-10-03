// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "MiniVehiclePawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UCameraComponent;

UCLASS()
class MINIDRIVINGSIMULATOR_API AMiniVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMiniVehiclePawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void OnThrottle(const struct FInputActionValue& Value);
    void OnBrake(const struct FInputActionValue& Value);
    void OnSteering(const struct FInputActionValue& Value);
    void OnHandbrake(const struct FInputActionValue& Value);
	void OnToggleCamera(const struct FInputActionValue& Value);

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* VehicleMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* ThrottleAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* BrakeAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SteeringAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* HandbrakeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ToggleCameraAction;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* BackCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* FrontCamera;
};
