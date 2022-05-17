// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class GAP6003_A1P1_1808896_API AHealthPickUp : public APickup
{
	GENERATED_BODY()

public:
	AHealthPickUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
