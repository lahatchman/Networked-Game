// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionPoint.h"
#include "GAP6003_A1P1_1808896Character.h"
#include "Collectable.h"

// Sets default values
ACollectionPoint::ACollectionPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("collisionComponent"));
	RootComponent = collisionComponent;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMeshComponent"));
	staticMeshComponent->SetupAttachment(RootComponent);
	staticMeshComponent->SetCollisionProfileName("BlockAll");
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACollectionPoint::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectionPoint::OnBeginOverlap);
}

// Called every frame
void ACollectionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectionPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAP6003_A1P1_1808896Character* chr = Cast<AGAP6003_A1P1_1808896Character>(OtherActor);

	if (chr && chr->IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit chr = %s"), *chr->GetName());
		chr->Collection(this);
	}
}
