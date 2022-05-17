// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Enums.h"
#include "CollectableSpawnVolume.h"
#include "ObstacleSpawnVolume.h"
#include "HealthPickUpSpawnVolume.h"
#include "GAP6003_A1P1_1808896GameMode.generated.h"

UCLASS(minimalapi)
class AGAP6003_A1P1_1808896GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAP6003_A1P1_1808896GameMode();

	// Override BeginPlay, since we need that to recreate the BP version 
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* newPC) override;

	int maxNumOfPlayers;

	UPROPERTY(EditDefaultsOnly, Category = Collectable)
		TSubclassOf<class ACollectable> redCollectableClass;
	UPROPERTY(EditDefaultsOnly, Category = Collectable)
		TSubclassOf<class ACollectable> blueCollectableClass;
	UPROPERTY(EditDefaultsOnly, Category = Collectable)
		TSubclassOf<class ACollectable> greenCollectableClass;
	UPROPERTY(EditDefaultsOnly, Category = Collectable)
		TSubclassOf<class ACollectable> yellowCollectableClass;

	void spawnRandomCollectables();
	void spawnCollectables(ACollectableSpawnVolume* spawnVol, EColours colour);

	ACollectableSpawnVolume* leftVol;
	ACollectableSpawnVolume* rightVol;

	void SpawnCollectableLeft();
	void SpawnCollectableRight();

	AObstacleSpawnVolume* obstacleLeftVol;
	AObstacleSpawnVolume* obstacleRightVol;

	UPROPERTY(EditDefaultsOnly, Category = Obstacle)
		TSubclassOf<class AObstacle> obstacleClass;

	void spawnRandomObstacles();
	void spawnObstacles(AObstacleSpawnVolume* spawnVol);

	void SpawnObstaclesLeft();
	void SpawnObstaclesRight();

	UPROPERTY(EditDefaultsOnly, Category = HealthPickUp)
		TSubclassOf<class AHealthPickUp> healthPickUpClass;

	void spawnRandomHealthPickUps();
	void spawnHealthPickUps(AHealthPickUpSpawnVolume* spawnVol);

	AHealthPickUpSpawnVolume* healthPickUpLeftVol;
	AHealthPickUpSpawnVolume* healthPickUpRightVol;

	void gameOver(int playerPos);
};



