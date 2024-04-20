// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/LobbyHUDWidget.h"

#include "Blueprint/UserWidget.h"
#include "UMG/PlayersLobbyWidget.h"

void ALobbyHUDWidget::CreatePlayersLobbyWidget(const FName& LobbyName, const float& DelaySessionToStart, const int32& PlayerNum)
{
	PlayersLobbyWidget = CreateWidget<UPlayersLobbyWidget>(GetOwningPlayerController(), PlayersLobbyWidgetClass);
	if (PlayersLobbyWidget)
	{
		PlayersLobbyWidget->SetLobbyName(LobbyName);
		PlayersLobbyWidget->SetSessionToStart(DelaySessionToStart);
		PlayersLobbyWidget->SetMaxPlayers(PlayerNum);
		PlayersLobbyWidget->AddToViewport();
	}
}

void ALobbyHUDWidget::UpdatePlayersLobbyWidget(const float& DelaySessionToStart)
{
	if (PlayersLobbyWidget)
	{
		PlayersLobbyWidget->SetSessionToStart(DelaySessionToStart);
	}
}
