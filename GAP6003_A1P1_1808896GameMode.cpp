// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAP6003_A1P1_1808896GameMode.h"
#include "GAP6003_A1P1_1808896Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameState.h"
#include "EngineUtils.h" //for ActorIterator
#include "Collectable.h"
#include "Obstacle.h"
#include "HealthPickUp.h"
#include "GameFramework/PlayerState.h"
#include "GAP6003_A1P1_1808896_GameState.h"


AGAP6003_A1P1_1808896GameMode::AGAP6003_A1P1_1808896GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	maxNumOfPlayers = 2;
	leftVol = nullptr;
	rightVol = nullptr;
	obstacleLeftVol = nullptr;
	obstacleRightVol = nullptr;
}

void AGAP6003_A1P1_1808896GameMode::BeginPlay()
{	//needed, else Character does NOT appear!!!
	Super::BeginPlay();
}

void AGAP6003_A1P1_1808896GameMode::PostLogin(APlayerController* newPC)
{
	Super::PostLogin(newPC);

	AGameStateBase* gs = GetGameState<AGameStateBase>();

	if (gs && gs->PlayerArray.Num() == maxNumOfPlayers) 
	{
		UE_LOG(LogTemp, Warning, TEXT("PostLogin Num players: %d"), gs->PlayerArray.Num());
		spawnRandomCollectables();
		spawnRandomObstacles();
		spawnRandomHealthPickUps();
	}
}

void AGAP6003_A1P1_1808896GameMode::spawnRandomCollectables()
{ //called when game started
	TArray<EColours>randColours;
	int noc = (int)getEnumColoursSize(), r;

	for (int i = 0; i < noc; i++) 
	{	//for each colour
		do 
		{	//randomly select colours
			r = FMath::Pow(2.0f, FMath::RandRange(0, noc - 1));
		} 
		while (randColours.Contains(EColours(r)));

		randColours.Add(EColours(r));
	}

	for (TActorIterator<ACollectableSpawnVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* actor = *ActorItr;
		if (ActorItr->Tags.Num() > 0) 
		{	//has a tag
			FString tag = ActorItr->Tags[0].ToString();

			if (tag == "left") leftVol = *ActorItr;
			else rightVol = *ActorItr;
		}
	}

	spawnCollectables(leftVol, randColours[0]);
	spawnCollectables(rightVol, randColours[0]);
}

void AGAP6003_A1P1_1808896GameMode::spawnCollectables(ACollectableSpawnVolume* spawnVol, EColours colour)
{
	if (spawnVol) 
	{
		switch (colour) 
		{
		case EColours::EC_Red:
			spawnVol->spawnAnActor(redCollectableClass);
			break;

		case  EColours::EC_Blue:
			spawnVol->spawnAnActor(blueCollectableClass);
			break;

		case  EColours::EC_Green:
			spawnVol->spawnAnActor(greenCollectableClass);
			break;

		case  EColours::EC_Yellow:
			spawnVol->spawnAnActor(yellowCollectableClass);
			break;

		default:
			break;
		}
	}
}

void AGAP6003_A1P1_1808896GameMode::SpawnCollectableLeft()
{
	TArray<EColours>randColours;
	int noc = (int)getEnumColoursSize(), r;

	for (int i = 0; i < noc; i++)
	{	//for each colour
		do
		{	//randomly select colours
			r = FMath::Pow(2.0f, FMath::RandRange(0, noc - 1));
		} while (randColours.Contains(EColours(r)));

		randColours.Add(EColours(r));
	}

	spawnCollectables(leftVol, randColours[0]);
}

void AGAP6003_A1P1_1808896GameMode::SpawnCollectableRight()
{
	TArray<EColours>randColours;
	int noc = (int)getEnumColoursSize(), r;

	for (int i = 0; i < noc; i++)
	{	//for each colour
		do
		{	//randomly select colours
			r = FMath::Pow(2.0f, FMath::RandRange(0, noc - 1));
		} while (randColours.Contains(EColours(r)));

		randColours.Add(EColours(r));
	}

	spawnCollectables(rightVol, randColours[0]);
}


void AGAP6003_A1P1_1808896GameMode::spawnRandomObstacles() 
{ //called when game started

	for (TActorIterator<AObstacleSpawnVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* actor = *ActorItr;

		if (ActorItr->Tags.Num() > 0) 
		{	//has a tag
			FString tag = ActorItr->Tags[0].ToString();
			if (tag == "left")
				obstacleLeftVol = *ActorItr;
			else
				obstacleRightVol = *ActorItr;
		}
	}

	for (int i = 0; i < 10; i++) 	//for left vol
		spawnObstacles(obstacleLeftVol);

	for (int i = 0; i < 10; i++) 	//for right vol
		spawnObstacles(obstacleRightVol);
}

void AGAP6003_A1P1_1808896GameMode::spawnObstacles(AObstacleSpawnVolume* spawnVol)
{
	if (spawnVol)
	{
		spawnVol->spawnAnActor(obstacleClass);
	}
}

void AGAP6003_A1P1_1808896GameMode::SpawnObstaclesLeft()
{
	spawnObstacles(obstacleLeftVol);
}

void AGAP6003_A1P1_1808896GameMode::SpawnObstaclesRight()
{
	spawnObstacles(obstacleRightVol);
}

void AGAP6003_A1P1_1808896GameMode::spawnRandomHealthPickUps()
{
	for (TActorIterator<AHealthPickUpSpawnVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* actor = *ActorItr;

		if (ActorItr->Tags.Num() > 0)
		{	//has a tag
			FString tag = ActorItr->Tags[0].ToString();
			if (tag == "left")
				healthPickUpLeftVol = *ActorItr;
			else
				healthPickUpRightVol = *ActorItr;
		}
	}

	for (int i = 0; i < 2; i++) 	//for left vol
		spawnHealthPickUps(healthPickUpLeftVol);

	for (int i = 0; i < 2; i++) 	//for right vol
		spawnHealthPickUps(healthPickUpRightVol);
}

void AGAP6003_A1P1_1808896GameMode::spawnHealthPickUps(AHealthPickUpSpawnVolume* spawnVol)
{
	if (spawnVol)
	{
		spawnVol->spawnAnActor(healthPickUpClass);
	}
}

void AGAP6003_A1P1_1808896GameMode::gameOver(int playerPos)
{
	AGAP6003_A1P1_1808896_GameState* gs = GetGameState<AGAP6003_A1P1_1808896_GameState>();  //get server character
	AGAP6003_A1P1_1808896Character* chr = Cast<AGAP6003_A1P1_1808896Character>(gs->PlayerArray[0]->GetPawn());

	chr->MC_GameOver(playerPos);
}

