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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	
	for (auto* Actor : AllActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor->GetLevel()->GetOuter()->GetName() :  %s"), *Actor->GetLevel()->GetOuter()->GetName());
		if (Actor->GetLevel()->GetOuter()->GetName() == "01")
		{
			TArray<UStaticMeshComponent*> Components;
			Actor->GetComponents<UStaticMeshComponent>(Components);
			for (int32 i = 0; i < Components.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("Components[i] :  %s"), *Components[i]->GetMaterial(i)->GetName());
			}
			UE_LOG(LogTemp, Warning, TEXT("Actor->GetLevel() :  %s"), *Actor->GetName());

		}
	}
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

