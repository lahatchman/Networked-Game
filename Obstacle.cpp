// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "GAP6003_A1P1_1808896Character.h"

// Sets default values
AObstacle::AObstacle()
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
	bReplicates = true;
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnBeginOverlap);
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAP6003_A1P1_1808896Character* chr = Cast<AGAP6003_A1P1_1808896Character>(OtherActor);
	if (chr && chr->IsLocallyControlled()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap chr = %s"), *chr->GetName());
		chr->UpdateHealth(chr->health - 10);
	}

}

