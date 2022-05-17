// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
#include "GAP6003_A1P1_1808896Character.h"

ACollectable::ACollectable() 
{
	bReplicates = true;
}


void ACollectable::BeginPlay()
{
	Super::BeginPlay();

	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnBeginOverlap);
	collisionComponent->OnComponentHit.AddDynamic(this, &ACollectable::OnHit);
}

//Note: collisioncomp should have Overlap set for Pawn (NOT Block)
void ACollectable::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAP6003_A1P1_1808896Character* chr = Cast<AGAP6003_A1P1_1808896Character>(OtherActor);
	if (chr && onFloor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap chr = %s"), *chr->GetName());
		onFloor = false;
		chr->grab(this, SweepResult.BoneName);
	}
}

//Note: Floor & Key need to have coll comps: Simulation Generates Hit Events set On
void ACollectable::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName().Find("Floor") >= 0)
		onFloor = true;
}
