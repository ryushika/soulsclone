// Fill out your copyright notice in the Description page of Project Settings.


#include "Sessions/MenuWidget.h"
#include "Sessions/SessionsSubsystem.h"
#include "Components/Button.h"
#include "OnlineSubsystemUtils.h"


void UMenuWidget::SetupMenu(int32 NumOfPublicConnections, FString LobbyPath)
{
	NumberOfPublicConnections = NumOfPublicConnections;
	LobbyMapPath = LobbyPath + "?listen";

	AddToViewport();

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		SessionsSubsystem = GameInstance->GetSubsystem<USessionsSubsystem>();
	}
	
	if (SessionsSubsystem)
	{
		SessionsSubsystem->OnSessionsSubsCreateSessionComplete.AddUObject(this, &UMenuWidget::OnCreateSessionComplete);
		//SessionsSubsystem->OnSessionsSubsFindSessionsComplete.AddUObject(this, &UMenuWidget::OnFindSessionsComplete);
		SessionsSubsystem->OnSessionsSubsJoinSessionComplete.AddUObject(this, &UMenuWidget::OnJoinSessionComplete);
	}
}

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMenuWidget::HostButtonClicked);
	}

	if (FindButton)
	{
		FindButton->OnClicked.AddDynamic(this, &UMenuWidget::FindButtonClicked);
	}
}

void UMenuWidget::NativeDestruct()
{
	RemoveFromParent();

	if (GetWorld())
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(true);
		}
	}

	Super::NativeDestruct();
}

void UMenuWidget::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if (SessionsSubsystem)
	{
		SessionsSubsystem->CreateSession(NumberOfPublicConnections);
	}
}

void UMenuWidget::FindButtonClicked()
{
	FindButton->SetIsEnabled(false);
	if (SessionsSubsystem)
	{
		SessionsSubsystem->FindSessions(100);
	}
}

void UMenuWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GetWorld())
		{
			GetWorld()->ServerTravel(LobbyMapPath);
		}
	}
	else
	{
		HostButton->SetIsEnabled(true);
	}
}

void UMenuWidget::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	if (!SessionsSubsystem)
	{
		return;
	}

	for (const auto& Result : SearchResults)
	{
		if (GEngine)
		{
			FString SessionName = "";
			Result.Session.SessionSettings.Get(FName("SessionName"), SessionName);
			if (SessionName != "")
			{
				FoundSessions.Add(FName(SessionName), Result);
			}
		}
	}

	if (FoundSessions.Num() != 0)
	{
		for (const auto& Session : FoundSessions)
		{
			SessionsSubsystem->JoinSession(Session.Value, Session.Key);
			return;
		}
	}

	if (!bWasSuccessful || SearchResults.Num() == 0)
	{
		FindButton->SetIsEnabled(true);
	}
}

void UMenuWidget::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result, const FName& SessionName)
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

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		FindButton->SetIsEnabled(true);
	}
}
