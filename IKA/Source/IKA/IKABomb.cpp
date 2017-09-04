// Fill out your copyright notice in the Description page of Project Settings.

#include "IKABomb.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "IKA.h"
#include "IKACharacter.h"

// Sets default values
AIKABomb::AIKABomb()
{

}

AIKABomb::AIKABomb(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BombCollisionComp"));
	BoxComponent->InitBoxExtent(FVector(25, 25, 25));
	BoxComponent->SetCollisionObjectType(COLLISION_BOMB);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(COLLISION_BLAST, ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = BoxComponent;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BombMesh"));
	Mesh->SetupAttachment(RootComponent);

	BlastTime = 3.f;
	BlastDuration = 1.f;
	BlastRange = 400.f;
	Exploding = false;
	BlastFxSpawned = false;
	TickTimer = 1.2f;
	RemoteTrigger = false;
}

// Called when the game starts or when spawned
void AIKABomb::BeginPlay()
{
	Super::BeginPlay();
	
	if (!RemoteTrigger)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AIKABomb::OnBlastStart, FMath::Max(0.1f, BlastTime), false);
	}
}

// Called every frame
void AIKABomb::Tick(float DeltaTime)
{
	TickTimer += DeltaTime;

	if (TickTimer > 1.f)
	{
		Super::Tick(DeltaTime);

		if (Exploding)
		{
			TArray<FVector> BlastEffectEndPoints;
			for (const FVector& Direction : BlastDirection)
			{
				FVector EndPoint = GetActorLocation() + BlastRange * Direction;
				TArray<struct FHitResult> MultiHits;
				BlastTrace(MultiHits, GetActorLocation(), GetActorLocation() + BlastRange * Direction);
				for (const FHitResult& HitResult : MultiHits)
				{
					if (HitResult.bBlockingHit)
					{
						EndPoint = HitResult.ImpactPoint;
					}
					else
					{
						AActor* Actor = HitResult.Actor.Get();
						if (AIKACharacter* Character = Cast<AIKACharacter>(Actor))
						{
							Character->Die();
						}
						else if (AIKABomb* Bomb = Cast<AIKABomb>(Actor))
						{
							Bomb->Trigger();
						}
					}
				}
				BlastEffectEndPoints.Add(EndPoint);
			}

			SpawnBlastEffect(BlastEffectEndPoints);
		}

		TickTimer = 0.f;
	}
	
}

void AIKABomb::Trigger()
{
	OnBlastStart();
}

void AIKABomb::OnBlastStart()
{
	if (Exploding)
	{
		return;
	}

	Exploding = true;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AIKABomb::OnBlastEnd, FMath::Max(1.f, BlastDuration), false);
	Mesh->DestroyComponent();
}

// void AIKABomb::OnBlastEnd()
// {
// 	AIKACharacter * Pawn = GetInstigator<AIKACharacter>();
// 	if (Pawn)
// 	{
// 		Pawn->RestoreBombAmount();
// 	}
// 
// 	SetLifeSpan(0.1f);
// }

void AIKABomb::SpawnBlastEffect(const TArray<FVector>& EndPoints)
{
	if (BlastFxSpawned)
	{
		return;
	}

	const FVector Origin = GetActorLocation();
	for (const FVector& EndPoint : EndPoints)
	{
		UParticleSystemComponent* BlastPSC = UGameplayStatics::SpawnEmitterAtLocation(this, BlastFX, Origin);
		if (BlastPSC)
		{
			BlastPSC->SetVectorParameter(TrailTargetParam, EndPoint);
			BlastPSC->SetFloatParameter(TrailLifetimeParam, BlastDuration);
		}
	}

	if (BlastSound)
	{
		UGameplayStatics::PlaySound2D(this, BlastSound);
	}
	
	BlastFxSpawned = true;
}

bool AIKABomb::BlastTrace(TArray<struct FHitResult>& OutHits, const FVector& StartTrace, const FVector& EndTrace) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(BlastTrace), true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	return GetWorld()->LineTraceMultiByChannel(OutHits, StartTrace, EndTrace, COLLISION_BLAST, TraceParams);
}

