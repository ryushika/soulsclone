// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MenuWidget.generated.h"

class UButton;
class USessionsSubsystem;

UCLASS()
class MEMORYLAKE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetupMenu(int32 NumOfPublicConnections = 4, FString LobbyPath = "/Game/Maps/Lobby");

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void FindButtonClicked();

	FString LobbyMapPath;
	int32 NumberOfPublicConnections;

	USessionsSubsystem* SessionsSubsystem;

	TMap<FName, FOnlineSessionSearchResult> FoundSessions;

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, const FName& SessionName);
};
