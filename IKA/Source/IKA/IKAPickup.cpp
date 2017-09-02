// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickup.h"
#include "IKA.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "IKAPlayerCharacter.h"

// Sets default values
AIKAPickup::AIKAPickup()
{

}

AIKAPickup::AIKAPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UCapsuleComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionComp"));
	CollisionComp->InitCapsuleSize(40.0f, 50.0f);
	CollisionComp->SetCollisionObjectType(COLLISION_PICKUP);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	PickupPSC = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("PickupFX"));
	PickupPSC->bAutoActivate = false;
	PickupPSC->bAutoDestroy = false;
	PickupPSC->SetupAttachment(RootComponent);

	PickedUpBy = NULL;
	bIsActive = false;
}

void AIKAPickup::NotifyActorBeginOverlap(class AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	PickupOnTouch(Cast<AIKAPlayerCharacter>(Other));
}

// Called when the game starts or when spawned
void AIKAPickup::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPickup();
}

// Called every frame
void AIKAPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIKAPickup::SpawnPickup()
{
	bIsActive = true;
	PickedUpBy = NULL;
	OnSpawned();

	TSet<AActor*> OverlappingPawns;
	GetOverlappingActors(OverlappingPawns, AIKAPlayerCharacter::StaticClass());

	for (AActor* OverlappingPawn : OverlappingPawns)
	{
		PickupOnTouch(CastChecked<AIKAPlayerCharacter>(OverlappingPawn));
	}
}

void AIKAPickup::PickupOnTouch(class AIKAPlayerCharacter* Pawn)
{
	if (bIsActive && Pawn && Pawn->IsAlive() && !IsPendingKill())
	{
		if (CanBePickedUp(Pawn))
		{
			GivePickupTo(Pawn);
			PickedUpBy = Pawn;

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();
			}
		}
	}

}

void AIKAPickup::GivePickupTo(class AIKAPlayerCharacter* Pawn)
{
}

bool AIKAPickup::CanBePickedUp(class AIKAPlayerCharacter* Pawn) const
{
	return Pawn && Pawn->IsAlive();
}

void AIKAPickup::OnSpawned()
{
	if (ActiveFX)
	{
		PickupPSC->SetTemplate(ActiveFX);
		PickupPSC->ActivateSystem();
	}
	else
	{
		PickupPSC->DeactivateSystem();
	}

	if (SpawnSound)
	{
		UGameplayStatics::PlaySound2D(this, SpawnSound);
	}
}

void AIKAPickup::OnPickedUp()
{
	PickupPSC->DeactivateSystem();
	
	if (PickupSound && PickedUpBy)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}

	SetLifeSpan(1.f);
}

