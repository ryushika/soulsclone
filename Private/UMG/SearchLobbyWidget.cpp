// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/SearchLobbyWidget.h"
#include "Sessions/SessionsSubsystem.h"
#include "OnlineSubsystemUtils.h"

void USearchLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		SessionsSubsystem = GameInstance->GetSubsystem<USessionsSubsystem>();
	}
	
	if (SessionsSubsystem)
	{
		SessionsSubsystem->OnSessionsSubsCreateSessionComplete.AddUObject(this, &USearchLobbyWidget::OnCreateSessionComplete);
		SessionsSubsystem->OnSessionsSubsFindSessionsComplete.AddUObject(this, &USearchLobbyWidget::OnFindSessionsComplete);
		SessionsSubsystem->OnSessionsSubsJoinSessionComplete.AddUObject(this, &USearchLobbyWidget::OnJoinSessionComplete);
	}
}

void USearchLobbyWidget::FindLobby()
{
	if (!SessionsSubsystem)
	{
		return;
	}
	
	SessionsSubsystem->FindSessions(100);
}

void USearchLobbyWidget::CreateLobby(int32 NumOfPublicConnections, FString LobbyPath, FString Options)
{
	if (!SessionsSubsystem)
	{
		return;
	}

	MaxCountsOfPlayer = NumOfPublicConnections;
	LobbyMap = LobbyPath + Options;

	SessionsSubsystem->CreateSession(NumOfPublicConnections);
}

void USearchLobbyWidget::JoinLobby(FName LobbyName)
{
	FOnlineSessionSearchResult* OnlineSessionSearchResult = FoundSessions.Find(LobbyName);
	if (!OnlineSessionSearchResult || !SessionsSubsystem)
	{
		return;
	}

	SessionsSubsystem->JoinSession(*OnlineSessionSearchResult, LobbyName);
}

void USearchLobbyWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}

	if (!GetWorld())
	{
		return;
	}

	GetWorld()->ServerTravel(LobbyMap);

	LobbyCreated.Broadcast();
}

void USearchLobbyWidget::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults,
	bool bWasSuccessful)
{
	if (!SessionsSubsystem)
	{
		return;
	}

	FoundSessions.Empty();

	TArray<FLobbyInfo> LobbiesInfo;

	auto SessionInterface = Online::GetSessionInterface(GetWorld());
	if (!SessionInterface.IsValid())
	{
		return;
	}

	for (const auto& Result : SearchResults)
	{
		FLobbyInfo LobbyInfo;
		Result.Session.SessionSettings.Get(FName("SessionName"), LobbyInfo.LobbyName);
		LobbyInfo.MaxCountOfPlayer = Result.Session.SessionSettings.NumPublicConnections;
		Result.Session.SessionSettings.Get(FName("CurrentPlayersNum"), LobbyInfo.PlayersConnected);

		LobbiesInfo.Add(LobbyInfo);
		FoundSessions.Add(FName(LobbyInfo.LobbyName), Result);
	}
	
	LobbiesFound.Broadcast(LobbiesInfo);
}

void USearchLobbyWidget::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, const FName& SessionName)
{
	IOnlineSessionPtr SessionInteface = Online::GetSessionInterface(GetWorld());
	if (SessionInteface.IsValid() && GetGameInstance())
	{
		FString URL;
		SessionInteface->GetResolvedConnectString(SessionName, URL);

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);	
		}
	}
}
