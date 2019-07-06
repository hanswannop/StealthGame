// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	NucleusComp = CreateDefaultSubobject<USphereComponent>(TEXT("NucleusComp"));
	NucleusComp->SetSphereRadius(100);
	NucleusComp->SetupAttachment(MeshComp);

	NucleusComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::BeginOverlap);

	ActiveZoneComp = CreateDefaultSubobject<USphereComponent>(TEXT("ActiveZoneComp"));
	NucleusComp->SetSphereRadius(5000);
	ActiveZoneComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PlayEffects();
		OtherActor->Destroy();

		TArray<UPrimitiveComponent*> ComponentsToRappel;
		ActiveZoneComp->GetOverlappingComponents(ComponentsToRappel);

		for (auto& PrimComp : ComponentsToRappel)
		{
			if (PrimComp && PrimComp->IsSimulatingPhysics())
			{
				const float SphereRadius = ActiveZoneComp->GetScaledSphereRadius();
				const float ForceStrength = 15000;

				PrimComp->AddRadialForce(this->GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			}

		}
	}
}

void AFPSBlackHole::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ContactFX, GetActorLocation());
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> ComponentsToAttract;
	ActiveZoneComp->GetOverlappingComponents(ComponentsToAttract);

	for (auto& PrimComp : ComponentsToAttract)
	{
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = ActiveZoneComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;
			
			PrimComp->AddRadialForce(this->GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
		
	}
}


