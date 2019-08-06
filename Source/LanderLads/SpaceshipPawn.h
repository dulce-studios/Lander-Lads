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

	void HandleAxisBindings(const EAxis::Type AxisType, const float AxisValue);

	void MoveCameraLookHorizontal(float AxisValue);
	void MoveCameraLookVertical(float AxisValue);

	// Action bindings
	void MoveUpButtonPressed();
	void MoveDownButtonPressed();
	void MoveRightButtonPressed();
	void MoveLeftButtonPressed();
	void MoveForwardButtonPressed();
	void MoveBackwardButtonPressed();
	void MoveUpButtonReleased();
	void MoveDownButtonReleased();
	void MoveRightButtonReleased();
	void MoveLeftButtonReleased();
	void MoveForwardButtonReleased();
	void MoveBackwardButtonReleased();

	// Ship-specific fields
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	USpaceshipStaticMeshComponent* SpaceshipStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	int ShipThreshold;
	
	/** 
	See https://wiki.unrealengine.com/Blueprints,_Empower_Your_Entire_Team_With_BlueprintNativeEvents
	on overriding a parent C++ function in Blueprint
	**/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Overridable Property")
	void SetSpaceshipStaticMesh();
	void SetSpaceshipStaticMesh_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Overridable Property")
	void SetSpaceshipThreshold();
	void SetSpaceshipThreshold_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnCompHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

private:
	const float ClampAxisValue(float AxisValue);

	UCurveFloat* CurveFloat;

	float UpThrustStartTime;
	float DownThrustStartTime;
	float RightThrustStartTime;
	float LeftThrustStartTime;
	float ForwardThrustStartTime;
	float BackwardThrustStartTime;

	FVector ResultantForceVector;

	void ExplodeShip();
};
