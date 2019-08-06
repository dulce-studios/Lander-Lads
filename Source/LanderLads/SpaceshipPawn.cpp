// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "SpaceshipPawn.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = this->CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	this->SpaceshipStaticMeshComponent = this->CreateDefaultSubobject<USpaceshipStaticMeshComponent>(
		TEXT("SpaceshipStaticMeshComponent"));

	this->SpaceshipStaticMeshComponent->AttachToComponent(
		this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	this->ResultantForceVector = FVector(0);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(
		TEXT("CurveFloat'/Game/Misc/ThrusterJerk.ThrusterJerk'"));
	this->CurveFloat = CurveAsset.Object;

	this->SetSpaceshipStaticMesh();
	this->SetSpaceshipThreshold();
}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();

	this->SpaceshipStaticMeshComponent->SetNotifyRigidBodyCollision(true);
	this->SpaceshipStaticMeshComponent->OnComponentHit.AddDynamic(this, &ASpaceshipPawn::OnCompHit);
}

// Called every frame
void ASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SpaceshipStaticMeshComponent-> AddForce(ResultantForceVector);
}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveUp", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveUpButtonPressed);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveDownButtonPressed);
	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveRightButtonPressed);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveLeftButtonPressed);
	PlayerInputComponent->BindAction("MoveForward", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveForwardButtonPressed);
	PlayerInputComponent->BindAction("MoveBackward", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveBackwardButtonPressed);

	PlayerInputComponent->BindAction("MoveUp", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveUpButtonReleased);
	PlayerInputComponent->BindAction("MoveDown", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveDownButtonReleased);
	PlayerInputComponent->BindAction("MoveRight", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveRightButtonReleased);
	PlayerInputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveLeftButtonReleased);
	PlayerInputComponent->BindAction("MoveForward", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveForwardButtonReleased);
	PlayerInputComponent->BindAction("MoveBackward", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveBackwardButtonReleased);

	PlayerInputComponent->BindAxis("ForwardBackward", this, &ASpaceshipPawn::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ASpaceshipPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis("UpDown", this, &ASpaceshipPawn::MoveUpDown);
	PlayerInputComponent->BindAxis("CameraLookHorizontal", this, &ASpaceshipPawn::MoveCameraLookHorizontal);
	PlayerInputComponent->BindAxis("CameraLookVertical", this, &ASpaceshipPawn::MoveCameraLookVertical);
}

void ASpaceshipPawn::MoveUpButtonPressed()
{	
	this->UpThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
	this->SpaceshipStaticMeshComponent->GetBumThrusterParticleSystemComponent()->SetActive(true);
}

void ASpaceshipPawn::MoveUpButtonReleased()
{
	this->UpThrustStartTime = 0.0f;
	this->SpaceshipStaticMeshComponent->GetBumThrusterParticleSystemComponent()->SetActive(false);
}

void ASpaceshipPawn::MoveDownButtonPressed()
{
	this->DownThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveDownButtonReleased()
{
	this->DownThrustStartTime = 0.0f;
}

void ASpaceshipPawn::MoveRightButtonPressed()
{
	this->UpThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveRightButtonReleased()
{
	this->UpThrustStartTime = 0.0f;
}

void ASpaceshipPawn::MoveLeftButtonPressed()
{
	this->LeftThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveLeftButtonReleased()
{
	this->LeftThrustStartTime = 0.0f;
}

void ASpaceshipPawn::MoveForwardButtonPressed()
{
	this->ForwardThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveForwardButtonReleased()
{
	this->ForwardThrustStartTime = 0.0f;
}

void ASpaceshipPawn::MoveBackwardButtonPressed()
{
	this->BackwardThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveBackwardButtonReleased()
{
	this->BackwardThrustStartTime = 0.0f;
}


void ASpaceshipPawn::MoveForwardBackward(float AxisValue)
{
	this->HandleAxisBindings(EAxis::X, AxisValue);
}

void ASpaceshipPawn::MoveLeftRight(float AxisValue)
{
	this->HandleAxisBindings(EAxis::Y, AxisValue);
}

void ASpaceshipPawn::MoveUpDown(float AxisValue)
{
	this->HandleAxisBindings(EAxis::Z, AxisValue);
	
}

void ASpaceshipPawn::HandleAxisBindings(EAxis::Type Axis, float AxisValue)
{
	const float ClampedAxisValue = this->ClampAxisValue(AxisValue);

	float Thrust;
	float TimeDifference;

	switch (Axis)
	{
	case EAxis::X:
		TimeDifference = ClampedAxisValue > 0.0f ?
			(this->GetWorld()->GetRealTimeSeconds() - this->ForwardThrustStartTime) :
			(this->GetWorld()->GetRealTimeSeconds() - this->BackwardThrustStartTime);
		Thrust = 490 * 4000;
		break;
	case EAxis::Y:
		TimeDifference = ClampedAxisValue > 0.0f ?
			(this->GetWorld()->GetRealTimeSeconds() - this->RightThrustStartTime) :
			(this->GetWorld()->GetRealTimeSeconds() - this->LeftThrustStartTime);
		Thrust = 490 * 4000;
		break;
	case EAxis::Z:
		TimeDifference = ClampedAxisValue > 0.0f ?
			(this->GetWorld()->GetRealTimeSeconds() - this->UpThrustStartTime) :
			(this->GetWorld()->GetRealTimeSeconds() - this->DownThrustStartTime);
		Thrust = this->GetWorld()->GetGravityZ() * -1 * 4000;
		break;
	default:
		TimeDifference = 0.0f;
		Thrust = 0.0f;
	}

	const float Curvey = this->CurveFloat->GetFloatValue(TimeDifference);
	float ForceVectorMagnitude = Thrust * ClampedAxisValue * Curvey;
	this->ResultantForceVector.SetComponentForAxis(Axis, ForceVectorMagnitude);

}

void ASpaceshipPawn::MoveCameraLookHorizontal(float AxisValue)
{
	float ClampedAxisValue = ClampAxisValue(AxisValue);
	this->AddControllerYawInput(
		this->GetWorld()->GetDeltaSeconds() * 45 * ClampedAxisValue);
}

void ASpaceshipPawn::MoveCameraLookVertical(float AxisValue)
{
	float ClampedAxisValue = ClampAxisValue(AxisValue);
	this->AddControllerPitchInput(
		this->GetWorld()->GetDeltaSeconds() * -45 * ClampedAxisValue);
}

const float ASpaceshipPawn::ClampAxisValue(float AxisValue)
{
	constexpr float LowerClampBounds = -1.0f;
	constexpr float UpperClampBounds = 1.0f;
	return FMath::Clamp(AxisValue, LowerClampBounds, UpperClampBounds);
}

void ASpaceshipPawn::OnCompHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (HitComp == this->SpaceshipStaticMeshComponent) {
		if (NormalImpulse.X > this->ShipThreshold ||
			NormalImpulse.Y > this->ShipThreshold ||
			NormalImpulse.Z > this->ShipThreshold) {
			this->ExplodeShip();
		}
	}
}

void ASpaceshipPawn::ExplodeShip()
{
	this->SpaceshipStaticMeshComponent->GetExplosionParticleSystemComponent()->SetRelativeScale3D(FVector(15));
	this->SpaceshipStaticMeshComponent->SetVisibility(false);
	this->SpaceshipStaticMeshComponent->SetSimulatePhysics(false);
	this->SpaceshipStaticMeshComponent->GetExplosionParticleSystemComponent()->SetActive(true);
}

void ASpaceshipPawn::SetSpaceshipStaticMesh_Implementation()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Meshes/USSShippington.USSShippington'"));

	this->SpaceshipStaticMeshComponent->SetStaticMesh(MeshAsset.Object);
}

void ASpaceshipPawn::SetSpaceshipThreshold_Implementation()
{
	this->ShipThreshold = 300000;
}
