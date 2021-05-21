// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreaming.h"
#include "CustomLevelScriptActor.h"
#include "LevelManagerActor.generated.h"

UCLASS()
class UE4LS_API ALevelManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void EnableReadyUnloadLevel(const FString& LevelName)
	{
		if (ACustomLevelScriptActor* La = GetCustomLevelScriptActorByName(LevelName))
		{
			La->bReadyUnload = true;
		}
	}

	UFUNCTION(BlueprintCallable)
	void DisableReadyUnloadLevel(const FString& LevelName)
	{
		if (ACustomLevelScriptActor* La = GetCustomLevelScriptActorByName(LevelName))
		{
			La->bReadyUnload = false;
		}
	}

private:
	ACustomLevelScriptActor* GetCustomLevelScriptActorByName(const FString& LevelName)
	{
		for (const auto* Level : GetWorld()->GetLevels())
		{
			if (Level->GetOuter()->GetName() == LevelName)
			{
				return Cast<ACustomLevelScriptActor>(Level->GetLevelScriptActor());
			}
		}
		return nullptr;
	}
};
