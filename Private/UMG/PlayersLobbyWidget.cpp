// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/PlayersLobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sessions/LobbyPlayerController.h"

void UPlayersLobbyWidget::ReturnToMainMenu()
{
	ALobbyPlayerController* MyLobbyPlayerController = GetOwningPlayer<ALobbyPlayerController>();
	if (MyLobbyPlayerController && MyLobbyPlayerController->GetLocalRole() != ENetRole::ROLE_Authority)
	{
		MyLobbyPlayerController->DestroySession(LobbyName);
	}
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}
