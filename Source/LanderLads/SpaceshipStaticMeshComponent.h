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

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	UParticleSystemComponent* ExplosionParticleSystemComponent;

};
