// Copyright 2019 Dulce Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "SpaceshipStaticMeshComponent.h"

#include "SpaceshipPawn.generated.h"

UCLASS()
class LANDERLADS_API ASpaceshipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceshipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used for handling inputs
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	void MoveForwardBackward(float Scale);
	void MoveLeftRight(float Scale);
	void MoveUpDown(float Scale);
	void MoveCameraLookHorizontal(float Scale);
	void MoveCameraLookVertical(float Scale);

	// Getters
	USpaceshipStaticMeshComponent* GetSpaceshipStaticMeshComponent();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), BlueprintReadWrite)
	USpaceshipStaticMeshComponent* SpaceshipStaticMeshComponent;

};
