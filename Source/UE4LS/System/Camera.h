// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CustomGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "Camera.generated.h"


UCLASS()
class UE4LS_API ACamera : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:
	ACamera();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
};
