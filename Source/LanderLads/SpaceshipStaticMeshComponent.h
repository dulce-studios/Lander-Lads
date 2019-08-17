// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "SpaceshipStaticMeshComponent.generated.h"

UCLASS(Blueprintable)
class LANDERLADS_API USpaceshipStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	USpaceshipStaticMeshComponent();

	void AttachThrusters(UParticleSystemComponent*& ParticleSystemComponent, const FName& ComponentName);

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* ExplosionParticleSystemComponent;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* BumThrusterParticleSystemComponent;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* ForwardTiltThrusterParticleSystemComponent;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* BackwardTiltThrusterParticleSystemComponent;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* LeftTiltThrusterParticleSystemComponent;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
	UParticleSystemComponent* RightTiltThrusterParticleSystemComponent;

};
