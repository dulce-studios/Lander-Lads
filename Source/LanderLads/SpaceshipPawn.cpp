// Copyright 2019 Dulce Studios. All Rights Reserved.

#include "SpaceshipPawn.h"

// Sets default values
ASpaceshipPawn::ASpaceshipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	PlayerInputComponent->BindAxis("CameraLookX", this, &ASpaceshipPawn::MoveCameraLookX);
	PlayerInputComponent->BindAxis("CameraLookY", this, &ASpaceshipPawn::MoveCameraLookY);
}

void ASpaceshipPawn::MoveForwardBackward(float Scale)
{

}

void ASpaceshipPawn::MoveLeftRight(float Scale)
{

}

void ASpaceshipPawn::MoveUpDown(float Scale)
{

}

void ASpaceshipPawn::MoveCameraLookX(float Scale)
{
	
}

void ASpaceshipPawn::MoveCameraLookY(float Scale)
{

}
