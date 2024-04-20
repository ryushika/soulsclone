// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/LobbyPlayerController.h"
#include "Sessions/LobbyGameMode.h"
#include "UMG/LobbyHUDWidget.h"
#include "Sessions/SessionsSubsystem.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	bReplicates = true;
}

void ALobbyPlayerController::DestroySession(const FName& SessionName)
{
	USessionsSubsystem* SessionsSubsystem = GetGameInstance()->GetSubsystem<USessionsSubsystem>();
	if (SessionsSubsystem && SessionsSubsystem->SessionExists(SessionName))
	{
		SessionsSubsystem->DestroySession(SessionName);
	}
}

void ALobbyPlayerController::Client_CreatePlayerLobbyWidget_Implementation(const FName& LobbyName,
                                                                           const float& DelaySessionToStart,
                                                                           const int32& PlayerNum)
{
	ALobbyHUDWidget* MyHUDWidget = GetHUD<ALobbyHUDWidget>();
	if (MyHUDWidget)
	{
		MyHUDWidget->CreatePlayersLobbyWidget(LobbyName, DelaySessionToStart, PlayerNum);
	}
}

void ALobbyPlayerController::Client_UpdatePlayerLobbyWidget_Implementation(const float& DelaySessionToStart)
{
	ALobbyHUDWidget* MyHUDWidget = GetHUD<ALobbyHUDWidget>();
	if (MyHUDWidget)
	{
		MyHUDWidget->UpdatePlayersLobbyWidget(DelaySessionToStart);
	}
}

void ALobbyPlayerController::Server_StartLobby_Implementation()
{
	ALobbyGameMode* MyLobbyGameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
	if (!ensure(MyLobbyGameMode))
	{
		return;
	}

	MyLobbyGameMode->SessionStartAfterDelay(this);
}

void ALobbyPlayerController::Client_ChangeInputModeGameOnly_Implementation()
{
	SetInputMode(FInputModeGameOnly());
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FName SessionName = GetGameInstance()->GetSubsystem<USessionsSubsystem>()->GetCurrentSessionName();

	if (!HasAuthority())
	{
		DestroySession(SessionName);
	}

	Super::EndPlay(EndPlayReason);
}
