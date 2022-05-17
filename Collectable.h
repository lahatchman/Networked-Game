// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Enums.h"
#include "Collectable.generated.h"

/**
 * 
 */
UCLASS()
class GAP6003_A1P1_1808896_API ACollectable : public APickup
{
	GENERATED_BODY()
	
public:
	ACollectable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Colour)
		EColours CollectableColour;

	bool onFloor;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
