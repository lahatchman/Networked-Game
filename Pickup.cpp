// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionComponent"));
	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMeshComponent"));

	RootComponent = collisionComponent;
	collisionComponent->SetSimulatePhysics(true);
	collisionComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	staticMeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

