// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionsSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSessionsSubsOnCreateSessionComplete, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FSessionsSubsOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FSessionsSubsOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result, const FName& SessionName);

UCLASS()
class MEMORYLAKE_API USessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USessionsSubsystem();

	void CreateSession(int32 NumberOfPublicConnections);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult, const FName& SessionName);
	void UpdateSession(const FName& SessionName, int32 CurrentNumOfPlayers);
	void StartSession(const FName& SessionName);
	void DestroySession(const FName& SessionName);
	bool SessionExists(const FName& SessionName) const;
	const FName& GetCurrentSessionName() const { return CurrentSessionName; }
	const int32& MaxPlayers() const { return MaxPlayersInSession; }

	FSessionsSubsOnCreateSessionComplete OnSessionsSubsCreateSessionComplete;
	FSessionsSubsOnFindSessionsComplete OnSessionsSubsFindSessionsComplete;
	FSessionsSubsOnJoinSessionComplete OnSessionsSubsJoinSessionComplete;

	FTimerHandle& GetUpdateSessionTimerHandle() { return UpdateSessionTimerHandle; }
	const FTimerHandle& GetUpdateSessionTimerHandle() const { return UpdateSessionTimerHandle; }

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FTimerHandle UpdateSessionTimerHandle;

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	int32 NumberOfPublicConnectionsBeforeDeletetion;
	bool bWasSessionCreated = false;

	FName CurrentSessionName;
	int32 MaxPlayersInSession;
};
