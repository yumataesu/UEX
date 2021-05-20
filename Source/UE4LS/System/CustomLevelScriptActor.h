// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CustomLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UE4LS_API ACustomLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
		float DisolveProgress;

	UPROPERTY(BlueprintReadWrite)
		float DisolveMaxTime;

	UPROPERTY(BlueprintReadWrite)
		bool bReadyUnload;
};
