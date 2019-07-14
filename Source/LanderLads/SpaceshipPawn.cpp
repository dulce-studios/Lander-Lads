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
}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();
	
	this->SpaceshipStaticMeshComponent->OnComponentHit.AddDynamic(this, &ASpaceshipPawn::OnCompHit);
}

// Called every frame
void ASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->GetSpaceshipStaticMeshComponent()->AddForce(ResultantForceVector);
}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveUpButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveUpButtonPressed);
	PlayerInputComponent->BindAction("MoveDownButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveDownButtonPressed);
	PlayerInputComponent->BindAction("MoveRightButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveRightButtonPressed);
	PlayerInputComponent->BindAction("MoveLeftButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveLeftButtonPressed);
	PlayerInputComponent->BindAction("MoveForwardButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveForwardButtonPressed);
	PlayerInputComponent->BindAction("MoveBackwardButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveBackwardButtonPressed);

	PlayerInputComponent->BindAction("MoveUpButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveUpButtonReleased);
	PlayerInputComponent->BindAction("MoveDownButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveDownButtonReleased);
	PlayerInputComponent->BindAction("MoveRightButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveRightButtonReleased);
	PlayerInputComponent->BindAction("MoveLeftButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveLeftButtonReleased);
	PlayerInputComponent->BindAction("MoveForwardButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveForwardButtonReleased);
	PlayerInputComponent->BindAction("MoveBackwardButtonReleased", EInputEvent::IE_Released, this, &ASpaceshipPawn::MoveBackwardButtonReleased);

	PlayerInputComponent->BindAxis("ForwardBackward", this, &ASpaceshipPawn::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ASpaceshipPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis("UpDown", this, &ASpaceshipPawn::MoveUpDown);
	PlayerInputComponent->BindAxis("CameraLookHorizontal", this, &ASpaceshipPawn::MoveCameraLookHorizontal);
	PlayerInputComponent->BindAxis("CameraLookVertical", this, &ASpaceshipPawn::MoveCameraLookVertical);
}

void ASpaceshipPawn::MoveUpButtonPressed()
{	
	this->UpThrustStartTime = this->GetWorld()->GetRealTimeSeconds();
}

void ASpaceshipPawn::MoveUpButtonReleased()
{
	this->UpThrustStartTime = 0.0f;
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
	constexpr float RelativeXThrust = 490 * 4000;
	const float ClampedAxisValue = ClampAxisValue(AxisValue);

	const float TimeDifference = ClampedAxisValue > 0.0f ?
		(this->GetWorld()->GetRealTimeSeconds() - this->ForwardThrustStartTime) :
		(this->GetWorld()->GetRealTimeSeconds() - this->BackwardThrustStartTime);
	
	const float Curvey = this->CurveFloat->GetFloatValue(TimeDifference);
	const float ForwardBackwardVectorMagnitude = RelativeXThrust * ClampedAxisValue * Curvey;
	this->ResultantForceVector.SetComponentForAxis(EAxis::X, ForwardBackwardVectorMagnitude);
}

void ASpaceshipPawn::MoveLeftRight(float AxisValue)
{
	constexpr float RelativeYThrust = 490 * 4000;
	const float ClampedAxisValue = ClampAxisValue(AxisValue);

	const float TimeDifference = ClampedAxisValue > 0.0f ?
		(this->GetWorld()->GetRealTimeSeconds() - this->RightThrustStartTime) :
		(this->GetWorld()->GetRealTimeSeconds() - this->LeftThrustStartTime);

	const float Curvey = this->CurveFloat->GetFloatValue(TimeDifference);
	const float ForwardBackwardVectorMagnitude = RelativeYThrust * ClampedAxisValue * Curvey;
	this->ResultantForceVector.SetComponentForAxis(EAxis::Y, ForwardBackwardVectorMagnitude);
}

void ASpaceshipPawn::MoveUpDown(float AxisValue)
{
	const float Lift = this->GetWorld()->GetGravityZ() * -1 * 4000;
	const float ClampedAxisValue = ClampAxisValue(AxisValue);

	const float TimeDifference = ClampedAxisValue > 0.0f ? 
		(this->GetWorld()->GetRealTimeSeconds() - this->UpThrustStartTime) : 
		(this->GetWorld()->GetRealTimeSeconds() - this->DownThrustStartTime);

	const float Curvey = this->CurveFloat->GetFloatValue(TimeDifference);
	const float UpDownForceVectorMagnitude = Lift * ClampedAxisValue * Curvey;
	this->ResultantForceVector.SetComponentForAxis(EAxis::Z, UpDownForceVectorMagnitude);
	
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
	return FMath::Clamp(AxisValue, -1.0f, 1.0f);
}

USpaceshipStaticMeshComponent* ASpaceshipPawn::GetSpaceshipStaticMeshComponent()
{
	return this->SpaceshipStaticMeshComponent;
}

void ASpaceshipPawn::OnCompHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("SOMEEEBUDY"));
	if (HitComp == this->GetSpaceshipStaticMeshComponent()) {
		UE_LOG(LogTemp, Warning, TEXT("WE ARE HIT!!"));
	}
}