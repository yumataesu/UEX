// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomLevelScriptActor.h"

ACustomLevelScriptActor::ACustomLevelScriptActor()
	: DisolveMaxTime(2.f)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


ACustomLevelScriptActor::~ACustomLevelScriptActor()
{}


void ACustomLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}


void ACustomLevelScriptActor::UpdateDisolveProgress(float DeltaTime)
{
	if (bReadyUnload)
	{
		DisolveProgress -= DeltaTime;
	}
	else
	{
		DisolveProgress += DeltaTime;
	}

	DisolveProgress = FMath::Clamp(DisolveProgress, 0.f, DisolveMaxTime);
}

