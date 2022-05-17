// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GAP6003_A1P1_1808896_GameState.generated.h"

/**
 * 
 */
UCLASS()
class GAP6003_A1P1_1808896_API AGAP6003_A1P1_1808896_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGAP6003_A1P1_1808896_GameState();
	/** Marks the properties we wish to replicate */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = Timer)
		float gameTime;
};
