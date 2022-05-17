// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawnVolume.h"

// Sets default values
AObstacleSpawnVolume::AObstacleSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	whereToSpawn->SetBoxExtent(FVector(200.0, 200.0, 10.0));
	whereToSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AObstacleSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacleSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AObstacleSpawnVolume::getRandomPtInVolume()
{
	FVector spawnOrigin = whereToSpawn->Bounds.Origin;
	FVector spawnExtent = whereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void AObstacleSpawnVolume::spawnAnActor(TSubclassOf<class AActor> actor)
{
	if (GetWorld()) 
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector spawnLoc = getRandomPtInVolume();
		FRotator rot = FRotator::ZeroRotator;
		AActor* a = GetWorld()->SpawnActor<AActor>(actor, spawnLoc, rot, spawnParams);
	}
}

