// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"	//for random in vol
#include "HealthPickUpSpawnVolume.generated.h"

UCLASS()
class GAP6003_A1P1_1808896_API AHealthPickUpSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickUpSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* whereToSpawn;

	FVector getRandomPtInVolume();
	void spawnAnActor(TSubclassOf<class AActor>actor);

	UPROPERTY(EditDefaultsOnly, Category = Actor)
		TSubclassOf<class AActor> actorClass;
};
