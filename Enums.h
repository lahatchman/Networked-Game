// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)	//BlueprintType so BP can use
enum class EColours : uint8 
{
	EC_None = 0,  //enum MUST have a 0 entry
	EC_Red = 1 	UMETA(DisplayName = "red"),
	EC_Blue = 2 	UMETA(DisplayName = "blue"),
	EC_Green = 4	UMETA(DisplayName = "green"),
	EC_Yellow = 8	UMETA(DisplayName = "yellow"),
};

//static utility functions
static int32 getEnumColoursSize() 
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EColours"), true);
	return EnumPtr->NumEnums() - 2; // -2 as extra name & 'hidden' MAX entry
}

static FString getEnumColoursNameByValue(int32 colour) 
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EColours"), true);
	FString s = EnumPtr->GetDisplayNameTextByValue(colour).ToString();
	int p = s.Find("_");  //in packaged builds, EC_colour will be returned!
	if (p >= 0)
		s = s.Mid(p + 1);
	return s;
}

