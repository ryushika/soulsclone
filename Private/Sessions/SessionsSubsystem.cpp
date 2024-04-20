// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/SessionsSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"

USessionsSubsystem::USessionsSubsystem()
{
	SessionInterface = Online::GetSessionInterface(GetWorld());

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnFindSessionComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnJoinSessionComplete);
	OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnUpdateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnStartSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USessionsSubsystem::OnDestroySessionComplete);
}

void USessionsSubsystem::CreateSession(int32 NumberOfPublicConnections)
{
	if (!SessionInterface.IsValid() || !GetWorld())
	{
		return;
	}

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !PlayerController->PlayerState.Get())
	{
		return;
	}

	const FString SessionName = PlayerController->PlayerState.Get()->GetPlayerName();

	if (SessionInterface->GetNamedSession(FName(SessionName)))
	{
		bWasSessionCreated = true;
		NumberOfPublicConnectionsBeforeDeletetion = NumberOfPublicConnections;
		DestroySession(FName(SessionName));
	}

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->NumPublicConnections = NumberOfPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->BuildUniqueId = 1;
	LastSessionSettings->bIsLANMatch = false;
	LastSessionSettings->Set(FName("SessionName"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(FName("CurrentPlayersNum"), int32(0), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), FName(SessionName), *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		OnSessionsSubsCreateSessionComplete.Broadcast(false);
	}

	MaxPlayersInSession = NumberOfPublicConnections;
}

void USessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid() || !GetWorld())
	{
		return;
	}

	OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->bIsLanQuery = false;
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		return;
	}

	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
		OnSessionsSubsFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void USessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult, const FName& SessionName)
{
	if (!SessionInterface.IsValid() || !GetWorld())
	{
		return;
	}

	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		return;
	}

	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, SessionResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		OnSessionsSubsJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError, FName(""));
	}
}

void USessionsSubsystem::UpdateSession(const FName& SessionName, int32 CurrentNumOfPlayers)
{
	if (!SessionInterface.IsValid() || !GetWorld())
	{
		return;
	}

	OnUpdateSessionCompleteDelegateHandle = SessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

	FOnlineSessionSettings* SessionSettings = SessionInterface->GetSessionSettings(SessionName);
	if (!SessionSettings)
	{
		return;
	}

	SessionSettings->Set(FName("CurrentPlayersNum"), CurrentNumOfPlayers, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (!SessionInterface->UpdateSession(SessionName, *SessionSettings))
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
	}
}

void USessionsSubsystem::StartSession(const FName& SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	OnStartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

	if (!SessionInterface->StartSession(SessionName))
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	}
}

void USessionsSubsystem::DestroySession(const FName& SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(SessionName))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}
}

bool USessionsSubsystem::SessionExists(const FName& SessionName) const
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	return SessionInterface->GetNamedSession(SessionName) != nullptr;
}

void USessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	}

	OnSessionsSubsCreateSessionComplete.Broadcast(bWasSuccessful);
}

void USessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() == 0)
	{
		OnSessionsSubsFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	OnSessionsSubsFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void USessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	}

	CurrentSessionName = SessionName;

	OnSessionsSubsJoinSessionComplete.Broadcast(Result, SessionName);
}

void USessionsSubsystem::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
	}
}

void USessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		FOnlineSessionSettings* SessionSettings = SessionInterface->GetSessionSettings(SessionName);
		if (SessionSettings && bWasSuccessful)
		{
			SessionSettings->bAllowJoinInProgress = false;
			SessionInterface->UpdateSession(SessionName, *SessionSettings);
		}
	}
}

void USessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}
	if (bWasSessionCreated)
	{
		bWasSessionCreated = false;
		CreateSession(NumberOfPublicConnectionsBeforeDeletetion);
	}
}
