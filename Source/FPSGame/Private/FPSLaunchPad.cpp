// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{



	TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	TriggerComp->SetBoxExtent(FVector(75, 75, 50));
	TriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = TriggerComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(TriggerComp);

	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

	LaunchPower = 1500.0f;
	LaunchAngle = 45.0f;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchPad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());
}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchRotation = GetActorRotation();
	LaunchRotation.Pitch += LaunchAngle;
	FVector LaunchVelocity = LaunchRotation.Vector() * LaunchPower;

	AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
		PlayEffects();
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		PlayEffects();
	}
}



