#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "SearchLobbyWidget.generated.h"

USTRUCT(BlueprintType)
struct FLobbyInfo
{
	GENERATED_BODY()

public:
	FLobbyInfo()
	{}

	FLobbyInfo(FName LobbyName, int32 PingInMs)
		: LobbyName(LobbyName.ToString())
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LobbyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayersConnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxCountOfPlayer;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobby);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyFound, const TArray<FLobbyInfo>&, LobbyInfo);

class USessionsSubsystem;

UCLASS()
class MEMORYLAKE_API USearchLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void FindLobby();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void CreateLobby(int32 NumOfPublicConnections, FString LobbyPath, FString Options);

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void JoinLobby(FName LobbyName);

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FOnLobbyFound LobbiesFound;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FOnLobby LobbyCreated;

protected:
	virtual void NativeConstruct() override;

	TMap<FName, FOnlineSessionSearchResult> FoundSessions;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	int32 MaxCountsOfPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	FString LobbyMap;

private:
	TObjectPtr<USessionsSubsystem> SessionsSubsystem;

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, const FName& SessionName);
};
