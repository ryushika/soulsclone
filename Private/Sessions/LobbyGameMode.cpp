// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Sessions/LobbyPlayerController.h"
#include "Sessions/SessionsSubsystem.h"
#include "Sessions/LobbyPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::SessionStartAfterDelay(const APlayerController* Instagator)
{
	if (!SessionsSubsystem || !GetWorld())
	{
		return;
	}

	if (ReadyPlayerToPlay.IsEmpty())
	{
		ReadyPlayerToPlay.Add(Instagator);
	}
	else
	{
		for (const APlayerController* PlayerController : ReadyPlayerToPlay)
		{
			if (PlayerController == Instagator)
			{
				return;
			}
		}

		ReadyPlayerToPlay.Add(Instagator);
	}

	AGameStateBase* MyGameState = GetGameState<AGameStateBase>();
	if (!ensure(MyGameState))
	{
		return;
	}

	if (GetWorldTimerManager().IsTimerActive(SessionsSubsystem->GetUpdateSessionTimerHandle())
		&& ReadyPlayerToPlay.Num() != MyGameState->PlayerArray.Num())
	{
		return;
	}

	SessionsSubsystem->StartSession(SessionName);

	if (!GetWorldTimerManager().IsTimerActive(SessionsSubsystem->GetUpdateSessionTimerHandle()))
	{
		GetWorldTimerManager().SetTimer(SessionsSubsystem->GetUpdateSessionTimerHandle(), this, &ALobbyGameMode::StartSession, DelayNotReadySessionStart, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(SessionsSubsystem->GetUpdateSessionTimerHandle(), this, &ALobbyGameMode::StartSession, DelayReadySessionStart, false);
	}

	UpdatePlayerLobbyWidget(GetSessionStartTimer());
}

float ALobbyGameMode::GetSessionStartTimer() const
{
	if (!SessionsSubsystem)
	{
		return 0.f;
	}

	return GetWorldTimerManager().GetTimerRemaining(SessionsSubsystem->GetUpdateSessionTimerHandle());
}

void ALobbyGameMode::StartSession()
{
    AGameStateBase* MyGameState = GetGameState<AGameStateBase>();
    if (!ensure(MyGameState))
    {
        return;
    }
    
    for (APlayerState* PlayerState : MyGameState->PlayerArray)
    {
        ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(PlayerState->GetOwner());
    	if (!PlayerController)
    	{
    		continue;
    	}

    	PlayerController->Client_ChangeInputModeGameOnly();
    }

	if(SessionsSubsystem && GetWorld())
	{
		FString URL = MapName + "?listen";
		GetWorld()->ServerTravel(URL);
	}
}

void ALobbyGameMode::CreatePlayerLobbyWidget(APlayerController* NewPlayer, const FName& LobbyName,
	const float& SessionToStart, const int32& PlayerNum)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	if (LobbyPlayerController)
	{
		LobbyPlayerController->Client_CreatePlayerLobbyWidget(LobbyName, SessionToStart, PlayerNum);
	}
}

void ALobbyGameMode::UpdatePlayerLobbyWidget(const float& SessionToStart)
{
	AGameStateBase* MyGameState = GetGameState<AGameStateBase>();
	if (!MyGameState)
	{
		return;
	}

	for (APlayerState* PlayerState : MyGameState->PlayerArray)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(PlayerState->GetOwner());
		if (!LobbyPlayerController)
		{
			continue;
		}
			
		LobbyPlayerController->Client_UpdatePlayerLobbyWidget(SessionToStart);
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < PlayerLobbyLocations.Num(); i++)
	{
		FPlayerLobbyInfo LobbyPlayerInfo;
		LobbyPlayerInfo.PlayerTransform = PlayerLobbyLocations[i];
		LobbyPlayerInfo.bTransformSet = false;
		PlayerLobbyInfo.Add(LobbyPlayerInfo);
	}
}

FPlayerLobbyInfo ALobbyGameMode::GetPlayerLobbyInfo()
{
	const TArray<FPlayerLobbyInfo>& AvailableLocations = PlayerLobbyInfo.FilterByPredicate([](const FPlayerLobbyInfo& PlayerLobbyInfo)
		{
			return !PlayerLobbyInfo.bTransformSet; 
		});

	int32 RandomIndex = FMath::RandRange(0, AvailableLocations.Num() - 1);
	int32 IndexValid = INDEX_NONE;

	if (AvailableLocations.IsValidIndex(RandomIndex))
	{
		IndexValid = PlayerLobbyInfo.Find(AvailableLocations[RandomIndex]);
	}

	if (IndexValid != INDEX_NONE)
	{
		PlayerLobbyInfo[IndexValid].bTransformSet = true;
		return PlayerLobbyInfo[IndexValid];
	}

	return FPlayerLobbyInfo();
}

void ALobbyGameMode::SpawnPlayerMeshInLobby(AController* NewPlayer)
{
	const FPlayerLobbyInfo& PlayerInfo = GetPlayerLobbyInfo();
	AActor* PlayerMesh = GetWorld()->SpawnActor<AActor>(PlayerLobbyMesh,
		PlayerInfo.PlayerTransform.GetLocation(), PlayerInfo.PlayerTransform.GetRotation().Rotator());

	FSpawnedPlayerMeshInfo SpawnedPlayerMeshInfo;
	SpawnedPlayerMeshInfo.PlayerMesh = PlayerMesh;
	SpawnedPlayerMeshInfo.PlayerLobbyInfo = PlayerInfo;
	SpawnedPlayerMesh.Add(NewPlayer, SpawnedPlayerMeshInfo);
}

void ALobbyGameMode::RemovePlayerMeshFromLobby(AController* ExitingPlayer)
{
	if (SpawnedPlayerMesh.Contains(ExitingPlayer))
	{
		FPlayerLobbyInfo* PlayerInfo = PlayerLobbyInfo.FindByKey(SpawnedPlayerMesh[ExitingPlayer].PlayerLobbyInfo);
		if (PlayerInfo)
		{
			PlayerInfo->bTransformSet = false;
		}
		SpawnedPlayerMesh[ExitingPlayer].PlayerMesh->Destroy();
		SpawnedPlayerMesh.Remove(ExitingPlayer);
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	if (!SessionsSubsystem)
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			SessionsSubsystem = GameInstance->GetSubsystem<USessionsSubsystem>();
		}
	}

	if (PlayersNum > 0)
	{
		SpawnPlayerMeshInLobby(NewPlayer);
	}

	FName PlayerName = FName(NewPlayer->PlayerState.Get()->GetPlayerName());
	
	if (SessionsSubsystem && SessionsSubsystem->SessionExists(PlayerName))
	{
		SessionName = PlayerName;
	}

	PlayersNum++;

	SessionsSubsystem->UpdateSession(SessionName, PlayersNum);

	CreatePlayerLobbyWidget(NewPlayer, GetSessionName(), GetSessionStartTimer(), SessionsSubsystem->MaxPlayers());
}

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	PlayersNum--;
	FName PlayerName = FName(Exiting->PlayerState->GetPlayerName());

	if (PlayerName == SessionName && SessionsSubsystem && SessionsSubsystem->SessionExists(SessionName))
	{
		SessionsSubsystem->DestroySession(SessionName);
		Super::Logout(Exiting);
		return;
	}

	if (SessionsSubsystem)
	{
		SessionsSubsystem->UpdateSession(SessionName, PlayersNum);
	}

	RemovePlayerMeshFromLobby(Exiting);

	Super::Logout(Exiting);
}