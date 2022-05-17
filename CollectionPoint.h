// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enums.h"
#include "CollectionPoint.generated.h"

UCLASS()
class GAP6003_A1P1_1808896_API ACollectionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectionPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EColours collectableColourReqd;
	UPROPERTY(EditAnywhere)
		UBoxComponent* collisionComponent;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* staticMeshComponent;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
