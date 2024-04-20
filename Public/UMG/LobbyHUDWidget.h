// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUDWidget.generated.h"

class UPlayersLobbyWidget;

UCLASS()
class MEMORYLAKE_API ALobbyHUDWidget : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	UPlayersLobbyWidget* GetPlayersLobbyWidget() const { return PlayersLobbyWidget; }

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetPlayersLobbyWidget(UPlayersLobbyWidget* NewPlayersLobbyWidget) { PlayersLobbyWidget = NewPlayersLobbyWidget; }

	UFUNCTION(BlueprintCallable, Category = "Widgets")
	TSubclassOf<UPlayersLobbyWidget> GetPlayersLobbyWidgetClass() const { return PlayersLobbyWidgetClass; }
	
	void CreatePlayersLobbyWidget(const FName& LobbyName, const float& DelaySessionToStart, const int32& PlayerNum);
	void UpdatePlayersLobbyWidget(const float& DelaySessionToStart);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UPlayersLobbyWidget> PlayersLobbyWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayersLobbyWidget> PlayersLobbyWidgetClass;
};
