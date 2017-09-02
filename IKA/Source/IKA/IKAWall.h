// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IKAWall.generated.h"

UCLASS()
class IKA_API AIKAWall : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Mesh */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;
		
	
public:	
	// Sets default values for this actor's properties
	AIKAWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
