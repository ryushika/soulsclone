// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameInstance/MLGameInstance.h"
#include "MoviePlayer.h"
#include "Runtime/UMG/Public/UMG.h"

void UMLGameInstance::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMLGameInstance::BeginLoadingScreen);
}

void UMLGameInstance::BeginLoadingScreen(const FString& InMapName)
{
    if (IsRunningDedicatedServer()) return;

    FLoadingScreenAttributes Attributes;
    Attributes.bAutoCompleteWhenLoadingCompletes = false;
    Attributes.bMoviesAreSkippable = true;
    Attributes.MinimumLoadingScreenDisplayTime = 2.f;

    UUserWidget* LoadingScreen = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
    if (!LoadingScreen) return;

    Attributes.WidgetLoadingScreen = LoadingScreen->TakeWidget();

    IGameMoviePlayer* MoviePlayer = GetMoviePlayer();
    if (MoviePlayer)
    {
        MoviePlayer->SetupLoadingScreen(Attributes);
    }
}