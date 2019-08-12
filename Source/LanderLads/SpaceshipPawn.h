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
	void PitchShip(float AxisValue);
	void RollShip(float AxisValue);
	void ThrustShip(float AxisValue);

	void MoveCameraLookHorizontal(float AxisValue);
	void MoveCameraLookVertical(float AxisValue);

	// Ship-specific fields
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpaceshipStaticMeshComponent* SpaceshipStaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Gameplay", BlueprintReadWrite)
	int DamageThreshold = 300000;

	UPROPERTY(EditAnywhere, Category = "Physics", BlueprintReadWrite)
	float AngularDamping = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Physics", BlueprintReadWrite)
	float AngularAccelerationFactor = 1.0f / 16;

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
	UCurveFloat* CurveFloat;

	void ExplodeShip();
};
