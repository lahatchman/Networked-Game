// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Pickup.generated.h"

UCLASS()
class GAP6003_A1P1_1808896_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		FString name;

	UPROPERTY(EditAnywhere)
		UBoxComponent* collisionComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* pickupSound;
};
