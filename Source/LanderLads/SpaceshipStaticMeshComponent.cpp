// Copyright 2019 Dulce Studios. All Rights Reserved.


#include "SpaceshipStaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

constexpr float TargetArmLength = 1400;

USpaceshipStaticMeshComponent::USpaceshipStaticMeshComponent()
{
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));

	this->ExplosionParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticleSystemComponent"));
	this->ExplosionParticleSystemComponent->SetTemplate(ExplosionParticleSystem.Object);
	this->ExplosionParticleSystemComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	this->ExplosionParticleSystemComponent->SetAutoActivate(false);

	this->AttachThrusters(this->BumThrusterParticleSystemComponent, "BumThrusterParticleSystemComponent");
	this->AttachThrusters(this->ForwardTiltThrusterParticleSystemComponent, "ForwardTiltThrusterParticleSystemComponent");
	this->AttachThrusters(this->BackwardTiltThrusterParticleSystemComponent, "BackwardTiltThrusterParticleSystemComponent");
	this->AttachThrusters(this->LeftTiltThrusterParticleSystemComponent, "LeftTiltThrusterParticleSystemComponent");
	this->AttachThrusters(this->RightTiltThrusterParticleSystemComponent, "RightTiltThrusterParticleSystemComponent");

}

void USpaceshipStaticMeshComponent::AttachThrusters(UParticleSystemComponent*& ParticleSystemComponent, const FName& ComponentName)
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ThrusterParticleSystem(TEXT("ParticleSystem'/Game/Particles/P_ThrusterExhaust.P_ThrusterExhaust'"));

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(ComponentName);
	ParticleSystemComponent->SetTemplate(ThrusterParticleSystem.Object);
	ParticleSystemComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystemComponent->SetAutoActivate(false);
}