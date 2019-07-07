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

	this->ForwardBackwardForceVector = FVector(0);
	this->LeftRightForceVector = FVector(0);
	this->UpDownForceVector = FVector(0);

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

	FVector ResultantForceVector =
		this->ForwardBackwardForceVector + this->LeftRightForceVector + this->UpDownForceVector;

	this->GetSpaceshipStaticMeshComponent()->AddForce(ResultantForceVector);
}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveUpButtonPressed", EInputEvent::IE_Pressed, this, &ASpaceshipPawn::MoveUpButtonPressed);

	PlayerInputComponent->BindAxis("ForwardBackward", this, &ASpaceshipPawn::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ASpaceshipPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis("UpDown", this, &ASpaceshipPawn::MoveUpDown);
	PlayerInputComponent->BindAxis("CameraLookHorizontal", this, &ASpaceshipPawn::MoveCameraLookHorizontal);
	PlayerInputComponent->BindAxis("CameraLookVertical", this, &ASpaceshipPawn::MoveCameraLookVertical);
}

void ASpaceshipPawn::MoveUpButtonPressed()
{
	UWorld* World = this->GetWorld();
	if (World) {
		this->ShipThrustStartTime = World->GetRealTimeSeconds();
	}
}

void ASpaceshipPawn::MoveForwardBackward(float AxisValue)
{

}

void ASpaceshipPawn::MoveLeftRight(float AxisValue)
{

}

void ASpaceshipPawn::MoveUpDown(float AxisValue)
{
	UWorld* World = this->GetWorld();
	if (World) {
		const float CounterGravity = World->GetGravityZ() * -1;
		const float ClampedAxisValue = ClampAxisValue(AxisValue);
		const float TimeDifference = World->GetRealTimeSeconds() - this->ShipThrustStartTime;
		const float Curvey = this->CurveFloat->GetFloatValue(TimeDifference);
		float UpDownForceVectorMagnitude = CounterGravity * 4000 * ClampedAxisValue * Curvey;
		this->UpDownForceVector.SetComponentForAxis(EAxis::Z, UpDownForceVectorMagnitude);
	}
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
	if (HitComp == this->GetSpaceshipStaticMeshComponent()) {
		UE_LOG(LogTemp, Warning, TEXT("WE ARE HIT!!"));
	}
}