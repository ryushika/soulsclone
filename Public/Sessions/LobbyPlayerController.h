// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

UCLASS()
class MEMORYLAKE_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();
	
	UFUNCTION(Client, Reliable)
	void Client_CreatePlayerLobbyWidget(const FName& LobbyName, const float& DelaySessionToStart, const int32& PlayerNum);
	
	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerLobbyWidget(const float& DelaySessionToStart);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_StartLobby();

	UFUNCTION(Client, Reliable)
	void Client_ChangeInputModeGameOnly();

	void DestroySession(const FName& SessionName);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
