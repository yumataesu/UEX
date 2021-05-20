// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagerActor.h"

// Sets default values
ALevelManagerActor::ALevelManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

