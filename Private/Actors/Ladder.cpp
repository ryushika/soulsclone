// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Ladder.h"

// Sets default values
ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

