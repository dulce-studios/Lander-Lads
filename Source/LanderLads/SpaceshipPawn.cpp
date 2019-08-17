// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "SpaceshipPawn.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = this->CreateDefaultSubobject<USceneComponent>(
		TEXT("RootComponent"));
		
	this->SpaceshipStaticMeshComponent = this->CreateDefaultSubobject<USpaceshipStaticMeshComponent>(
		TEXT("SpaceshipStaticMeshComponent"));

	this->SpaceshipStaticMeshComponent->AttachToComponent(
		this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(
		TEXT("CurveFloat'/Game/Misc/ThrusterJerk.ThrusterJerk'"));
	this->CurveFloat = CurveAsset.Object;
}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();
	this->SpaceshipStaticMeshComponent->SetSimulatePhysics(true);
	this->SpaceshipStaticMeshComponent->SetAngularDamping(this->AngularDamping);
	this->SpaceshipStaticMeshComponent->SetNotifyRigidBodyCollision(true);
	this->SpaceshipStaticMeshComponent->OnComponentHit.AddDynamic(this, &ASpaceshipPawn::OnCompHit);
}

// Called every frame
void ASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine) {
		FString Velocity = FString::Printf(TEXT("Velocity: %f"), (this->SpaceshipStaticMeshComponent->GetComponentVelocity().Size()/100));
		GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Green, Velocity);
	}
}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Pitch", this, &ASpaceshipPawn::PitchShip);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceshipPawn::RollShip);
	PlayerInputComponent->BindAxis("Thrust", this, &ASpaceshipPawn::ThrustShip);
	PlayerInputComponent->BindAxis("CameraLookHorizontal", this, &ASpaceshipPawn::MoveCameraLookHorizontal);
	PlayerInputComponent->BindAxis("CameraLookVertical", this, &ASpaceshipPawn::MoveCameraLookVertical);
}

void ASpaceshipPawn::PitchShip(float AxisValue)
{
	const float AngularMagnitude = PI * this->AngularAccelerationFactor;
	const FVector LocalTorque(0, AxisValue * AngularMagnitude, 0);
	const FVector Torque = this->GetControlRotation().RotateVector(LocalTorque);
	this->SpaceshipStaticMeshComponent->AddTorqueInRadians(Torque, NAME_None, true);
}

void ASpaceshipPawn::RollShip(float AxisValue)
{
	const float AngularMagnitude = PI * this->AngularAccelerationFactor;
	const FVector LocalTorque(AxisValue * AngularMagnitude, 0, 0);
	const FVector Torque = this->GetControlRotation().RotateVector(LocalTorque);
	this->SpaceshipStaticMeshComponent->AddTorqueInRadians(Torque, NAME_None, true);
}

void ASpaceshipPawn::ThrustShip(float AxisValue)
{
	static float PreviousTime = 0.0f;
	static float PreviousAxisValue = 0.0f;

	const bool bAxisValueHasChanged = !FMath::IsNearlyEqual(AxisValue, PreviousAxisValue);

	const float Magnitude = -this->GetWorld()->GetGravityZ() * 4000; 
	const float CurrentTime = this->GetWorld()->GetTimeSeconds();
	const float TimeDifference = bAxisValueHasChanged ? 0.0f : CurrentTime - PreviousTime;
	const float ForceScale = this->CurveFloat->GetFloatValue(TimeDifference);
	const float Direction = AxisValue;
	const float Thrust = Direction * Magnitude * ForceScale;

	FRotator ShipWorldRotation = this->SpaceshipStaticMeshComponent->GetComponentRotation();
	FVector ThrustVector = ShipWorldRotation.RotateVector(FVector(0.0f, 0.0f, Thrust));
	this->SpaceshipStaticMeshComponent->AddForce(ThrustVector);
	this->SpaceshipStaticMeshComponent->BumThrusterParticleSystemComponent->SetActive(AxisValue > 0.0f);

	if (bAxisValueHasChanged)
		PreviousTime = CurrentTime;
	PreviousAxisValue = AxisValue;
}

void ASpaceshipPawn::MoveCameraLookHorizontal(float AxisValue)
{
	this->AddControllerYawInput(
		this->GetWorld()->GetDeltaSeconds() * 45 * AxisValue);
}

void ASpaceshipPawn::MoveCameraLookVertical(float AxisValue)
{
	this->AddControllerPitchInput(
		this->GetWorld()->GetDeltaSeconds() * -45 * AxisValue);
}

void ASpaceshipPawn::OnCompHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (HitComp == this->SpaceshipStaticMeshComponent) {
		if (NormalImpulse.X > this->DamageThreshold ||
			NormalImpulse.Y > this->DamageThreshold ||
			NormalImpulse.Z > this->DamageThreshold) {
			this->ExplodeShip();
		}
	}
}

void ASpaceshipPawn::ExplodeShip()
{
	this->SpaceshipStaticMeshComponent->ExplosionParticleSystemComponent->SetRelativeScale3D(FVector(15));
	this->SpaceshipStaticMeshComponent->SetVisibility(false);
	this->SpaceshipStaticMeshComponent->SetSimulatePhysics(false);
	this->SpaceshipStaticMeshComponent->ExplosionParticleSystemComponent->SetActive(true);
}