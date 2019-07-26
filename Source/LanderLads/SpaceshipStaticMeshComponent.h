// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "SpaceshipStaticMeshComponent.generated.h"

UCLASS()
class LANDERLADS_API USpaceshipStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	USpaceshipStaticMeshComponent();

	UParticleSystemComponent* GetExplosionParticleSystemComponent();
	UParticleSystemComponent* GetBumThrusterParticleSystemComponent();

private:
	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UParticleSystemComponent* ExplosionParticleSystemComponent;

	UPROPERTY()
	UParticleSystemComponent* BumThrusterParticleSystemComponent;

};
