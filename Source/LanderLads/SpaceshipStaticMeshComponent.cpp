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

	this->ExplosionParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticleSystemComponent"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleSystem(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	this->ExplosionParticleSystemComponent->SetTemplate(ExplosionParticleSystem.Object);
	this->ExplosionParticleSystemComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	this->ExplosionParticleSystemComponent->SetAutoActivate(false);

	// TODO: Need a better way to attach thrusters...
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ThrusterParticleSystem(TEXT("ParticleSystem'/Game/Particles/P_ThrusterExhaust.P_ThrusterExhaust'"));

	UBoxComponent* BumThruster = CreateDefaultSubobject<UBoxComponent>(TEXT("BumThruster"));
	BumThruster->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	BumThruster->SetRelativeTransform(FTransform(FRotator(-180, 0, 0), FVector(0, 0, -250), FVector(5)));
	BumThruster->SetVisibility(false);

	this->BumThrusterParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BumThrusterParticleSystemComponent"));
	this->BumThrusterParticleSystemComponent->SetTemplate(ThrusterParticleSystem.Object);
	this->BumThrusterParticleSystemComponent->AttachToComponent(BumThruster, FAttachmentTransformRules::KeepRelativeTransform);
	this->BumThrusterParticleSystemComponent->SetAutoActivate(false);
}

UParticleSystemComponent* USpaceshipStaticMeshComponent::GetExplosionParticleSystemComponent()
{
	return this->ExplosionParticleSystemComponent;
}

UParticleSystemComponent* USpaceshipStaticMeshComponent::GetBumThrusterParticleSystemComponent()
{
	return this->BumThrusterParticleSystemComponent;
}