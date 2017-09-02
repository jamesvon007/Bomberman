// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAWall.h"
#include "Components/BoxComponent.h"


// Sets default values
AIKAWall::AIKAWall()
{
}

AIKAWall::AIKAWall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UBoxComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WallCollisionComp"));
	CollisionComp->InitBoxExtent(FVector(51, 51, 51));
	CollisionComp->SetCollisionObjectType(ECC_WorldStatic);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = CollisionComp;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("WallMesh"));
	Mesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AIKAWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIKAWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

