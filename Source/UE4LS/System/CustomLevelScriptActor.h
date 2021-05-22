// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "CustomLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class UE4LS_API ACustomLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACustomLevelScriptActor();
	~ACustomLevelScriptActor();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateDisolveProgress(float DeltaTime);


	UPROPERTY(BlueprintReadOnly)
		float DisolveProgress;

	UPROPERTY(BlueprintReadWrite)
		float DisolveMaxTime;

	UPROPERTY(BlueprintReadOnly)
		bool bReadyUnload;


	TArray<AActor*> AllActors;
};
