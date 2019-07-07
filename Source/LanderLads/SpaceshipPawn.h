// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipStaticMeshComponent.h"

#include "SpaceshipPawn.generated.h"

UCLASS()
class LANDERLADS_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used for handling inputs
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	// Axis bindings
	void MoveForwardBackward(float AxisValue);
	void MoveLeftRight(float AxisValue);
	void MoveUpDown(float AxisValue);
	void MoveCameraLookHorizontal(float AxisValue);
	void MoveCameraLookVertical(float AxisValue);

	// Action bindings
	void MoveUpButtonPressed();

	UFUNCTION()
	void OnCompHit(
		UPrimitiveComponent* HitComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit);

	// Getters
	USpaceshipStaticMeshComponent* GetSpaceshipStaticMeshComponent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	const float ClampAxisValue(float AxisValue);

	UCurveFloat* CurveFloat;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	USpaceshipStaticMeshComponent* SpaceshipStaticMeshComponent;

	float ShipThrustStartTime;

	FVector ForwardBackwardForceVector;
	FVector UpDownForceVector;
	FVector LeftRightForceVector;
};