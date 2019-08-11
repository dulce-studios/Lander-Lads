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

	UParticleSystemComponent* GetExplosionParticleSystemComponent();
	UParticleSystemComponent* GetBumThrusterParticleSystemComponent();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UParticleSystemComponent* ExplosionParticleSystemComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UParticleSystemComponent* BumThrusterParticleSystemComponent;

};
