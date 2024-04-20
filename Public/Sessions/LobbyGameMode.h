// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"


USTRUCT(BlueprintType)
struct FPlayerLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform PlayerTransform;

	bool bTransformSet = false;

	bool operator==(const FPlayerLobbyInfo& Other) const
	{
		return PlayerTransform.Equals(Other.PlayerTransform);
	}
};

USTRUCT(BlueprintType)
struct FSpawnedPlayerMeshInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* PlayerMesh = nullptr;

	UPROPERTY()
	FPlayerLobbyInfo PlayerLobbyInfo;
};

class USessionsSubsystem;

UCLASS()
class MEMORYLAKE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();

	UFUNCTION(BlueprintCallable, Category = "Session")
	void SessionStartAfterDelay(const APlayerController* Instagator);

	UFUNCTION(BlueprintCallable, Category = "Session")
	float GetSessionStartTimer() const;

	UFUNCTION(BlueprintPure, Category = "Session")
	int32 GetPlayersNum() const { return PlayersNum; }

	UFUNCTION(BlueprintPure, Category = "Session")
	const FName& GetSessionName() const { return SessionName; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Map")
	FString MapName;

	UPROPERTY(EditDefaultsOnly, Category = "Session|Delay")
	float DelayNotReadySessionStart;

	UPROPERTY(EditDefaultsOnly, Category = "Session|Delay")
	float DelayReadySessionStart;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;

	UFUNCTION()
	void StartSession();
	
	void CreatePlayerLobbyWidget(APlayerController* NewPlayer, const FName& LobbyName, const float& SessionToStart, const int32& PlayerNum);

	void UpdatePlayerLobbyWidget(const float& SessionToStart);

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TArray<FTransform> PlayerLobbyLocations;

	UPROPERTY()
	TArray<FPlayerLobbyInfo> PlayerLobbyInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Map")
	TSubclassOf<AActor> PlayerLobbyMesh;

private:
	UPROPERTY()
	TObjectPtr<USessionsSubsystem> SessionsSubsystem = nullptr;

	UPROPERTY()
	TArray<const APlayerController*> ReadyPlayerToPlay; 
	
	FName SessionName;
	int32 PlayersNum = 0;

	TMap<AController*, FSpawnedPlayerMeshInfo> SpawnedPlayerMesh;

	FPlayerLobbyInfo GetPlayerLobbyInfo();
	void SpawnPlayerMeshInLobby(AController* NewPlayer);
	void RemovePlayerMeshFromLobby(AController* ExitingPlayer);
};
