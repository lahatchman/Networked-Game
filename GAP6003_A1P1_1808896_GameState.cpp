// Fill out your copyright notice in the Description page of Project Settings.


#include "GAP6003_A1P1_1808896_GameState.h"
#include "Net/UnrealNetwork.h" //for replication

AGAP6003_A1P1_1808896_GameState::AGAP6003_A1P1_1808896_GameState()
{
	gameTime = 0.0f;
}

void AGAP6003_A1P1_1808896_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Update replicated properties
	DOREPLIFETIME(AGAP6003_A1P1_1808896_GameState, gameTime);
}

