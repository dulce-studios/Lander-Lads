// Copyright 2019 Dulce Studios. All Rights Reserved.


#include "SpaceshipStaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

constexpr float TargetArmLength = 1400;

USpaceshipStaticMeshComponent::USpaceshipStaticMeshComponent()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Game/Meshes/USSShippington.USSShippington'"));
	this->SetStaticMesh(MeshAsset.Object);

	this->SpringArm = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	this->SpringArm->TargetArmLength = TargetArmLength;
	this->SpringArm->bInheritPitch = true;
	this->SpringArm->bInheritYaw = true;
	this->SpringArm->bUsePawnControlRotation = true;
	this->SpringArm->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	UCameraComponent* Camera = this->CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetFieldOfView(90);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	
	FTransform CameraTransform(FRotator(-20, 0, 0), FVector(0, 0, 300), FVector(1));
	Camera->SetRelativeTransform(CameraTransform);
	Camera->AttachToComponent(this->SpringArm, FAttachmentTransformRules::KeepRelativeTransform);

}
