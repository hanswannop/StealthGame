// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* TriggerComp;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* LaunchFX;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchPower;

	UPROPERTY(EditInstanceOnly, Category = "LaunchPad")
	float LaunchAngle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

	UFUNCTION()
	void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
