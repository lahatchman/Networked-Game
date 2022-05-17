// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickUp.h"
#include "GAP6003_A1P1_1808896Character.h"

AHealthPickUp::AHealthPickUp() 
{
	bReplicates = true;
}

void AHealthPickUp::BeginPlay()
{
	Super::BeginPlay();

	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickUp::OnBeginOverlap);
}

void AHealthPickUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGAP6003_A1P1_1808896Character* chr = Cast<AGAP6003_A1P1_1808896Character>(OtherActor);
	if (chr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap chr = %s"), *chr->GetName());
		chr->ChangeHealth(chr->health + 50);
		Destroy();
	}
}
