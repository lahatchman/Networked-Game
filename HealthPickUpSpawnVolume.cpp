// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUpSpawnVolume.h"

// Sets default values
AHealthPickUpSpawnVolume::AHealthPickUpSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a Box for the spawn volume.
	whereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	whereToSpawn->SetBoxExtent(FVector(200.0, 200.0, 10.0));
	whereToSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthPickUpSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPickUpSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AHealthPickUpSpawnVolume::getRandomPtInVolume()
{
	FVector spawnOrigin = whereToSpawn->Bounds.Origin;
	FVector spawnExtent = whereToSpawn->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void AHealthPickUpSpawnVolume::spawnAnActor(TSubclassOf<class AActor> actor)
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

