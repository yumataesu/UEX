// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Osc/OscMessage.h"

#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4LS_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Category = "Test", BlueprintImplementableEvent, BlueprintCallable)
		void NotifyReceivedOsc(FOscMessage Msg);

	UFUNCTION(Category = "Test", BlueprintImplementableEvent, BlueprintCallable)
		void NotifyLoadScene(const FString& LevelName);

	UFUNCTION(Category = "Test", BlueprintImplementableEvent, BlueprintCallable)
		void NotifyUnloadScene(const FString& LevelName);

	FMatrix Modelview;
	FMatrix Projection;
};
