// Fill out your copyright notice in the Description page of Project Settings.

#include "IKAPickup.h"
#include "IKA.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Pawn/IKACharacter.h"

// Sets default values
AIKAPickup::AIKAPickup()
{

}

AIKAPickup::AIKAPickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CollisionComp"));
	BoxComponent->InitBoxExtent(FVector(15, 15, 15));
	BoxComponent->SetCollisionObjectType(COLLISION_PICKUP);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(COLLISION_BLAST, ECR_Overlap);
	RootComponent = BoxComponent;

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
	PickupOnTouch(Cast<AIKACharacter>(Other));
}

// Destroy pickup
void AIKAPickup::Destroy()
{
	// prevent pickup from being destroyed as soon as spawn
	if (GetGameTimeSinceCreation() < 2.f)
	{
		return;
	}
	SetLifeSpan(0.1f);
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
	GetOverlappingActors(OverlappingPawns, AIKACharacter::StaticClass());

	for (AActor* OverlappingPawn : OverlappingPawns)
	{
		PickupOnTouch(CastChecked<AIKACharacter>(OverlappingPawn));
	}
}

void AIKAPickup::PickupOnTouch(class AIKACharacter* Pawn)
{
	if (bIsActive && Pawn && Pawn->IsAlive() && !IsPendingKill())
	{
		if (CanBePickedUp(Pawn))
		{
			PickedUpBy = Pawn;
			GivePickupTo();

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();
			}
		}
	}

}

void AIKAPickup::GivePickupTo()
{
}

bool AIKAPickup::CanBePickedUp(class AIKACharacter* Pawn) const
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

