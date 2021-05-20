// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/LevelScriptActor.h"
#include "Engine/LevelStreaming.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "UObject/UnrealType.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"

#include "CustomLevelScriptActor.h"
#include "CustomGameInstance.h"

#define IMGUI_API

#ifdef IMGUI_API
#define WITH_IMGUI 1
#else
#define WITH_IMGUI 0
#endif // IMGUI_API

#if WITH_IMGUI
#include <ImGuiModule.h>
#include <ImGuiDelegates.h>
#include <imgui.h>
#endif


#include "ImGuiActor.generated.h"

UCLASS()
class UE4LS_API AImGuiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImGuiActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_IMGUI
	void ImGuiTick();
#endif

	UPROPERTY(EditAnywhere)
		AActor* CaptureComponent2D;

	TArray<FString> SubLevelNames;

	TArray<FString> CurrentLevels;
	TArray<float> CameraPositions;
	TArray<float> CameraRotations;
};
