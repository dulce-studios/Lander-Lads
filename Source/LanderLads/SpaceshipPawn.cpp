// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "SpaceshipPawn.h"

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

}

// Called when the game starts or when spawned
void ASpaceshipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceshipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpaceshipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("ForwardBackward", this, &ASpaceshipPawn::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ASpaceshipPawn::MoveLeftRight);
	PlayerInputComponent->BindAxis("UpDown", this, &ASpaceshipPawn::MoveUpDown);
	PlayerInputComponent->BindAxis("CameraLookHorizontal", this, &ASpaceshipPawn::MoveCameraLookHorizontal);
	PlayerInputComponent->BindAxis("CameraLookVertical", this, &ASpaceshipPawn::MoveCameraLookVertical);
}

void ASpaceshipPawn::MoveForwardBackward(float AxisValue)
{

}

void ASpaceshipPawn::MoveLeftRight(float AxisValue)
{

}

void ASpaceshipPawn::MoveUpDown(float AxisValue)
{

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

USpaceshipStaticMeshComponent* ASpaceshipPawn::GetSpaceshipStaticMeshComponent()
{
	return this->SpaceshipStaticMeshComponent;
}
