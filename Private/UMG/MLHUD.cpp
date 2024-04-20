// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/MLHUD.h"
#include "Blueprint/UserWidget.h"
#include "UMG/ChoosenElementsWidget.h"
#include "UMG/MainHUDWidget.h"
#include "UMG/UpgradeWidget.h"
#include "Components/UpgradeComponent.h"

void AMLHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (MainWidgetClass) {
		MainWidget = CreateAndAddWidget<UMainHUDWidget>(MainWidgetClass);
	}
	
	if (UpgradeWidgetClass)
	{
		UpgradeWidget = CreateAndAddWidget<UUpgradeWidget>(UpgradeWidgetClass);
		if (UpgradeWidget)
		{
			UpgradeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

template<typename T>
T* AMLHUD::CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (!CreatedWidget) return nullptr;

	CreatedWidget->AddToViewport();

	return Cast<T>(CreatedWidget);
}

void AMLHUD::SetHealth(float HealthPercent)
{
	if (!MainWidget)
	{
		return;
	}

	MainWidget->SetHealth(HealthPercent);
}

void AMLHUD::SetFocusValue(int32 FocusValue)
{
	if (!MainWidget)
	{
		return;
	}

	MainWidget->SetFocus(FocusValue);
}

void AMLHUD::ShowUpgrades(UUpgradeComponent const* OwnerUpgradeComponent)
{
	if (!UpgradeWidget || !OwnerUpgradeComponent)
	{
		return;
	}

	TArray<FUpgradeStruct> AvailableUpgrades = OwnerUpgradeComponent->GetOptions();
	if (AvailableUpgrades.IsEmpty())
	{
		return;
	}
	UpgradeWidget->RemoveFromParent();
	if (UpgradeWidgetClass)
	{
		UpgradeWidget = CreateAndAddWidget<UUpgradeWidget>(UpgradeWidgetClass);
		if (UpgradeWidget)
		{
			UpgradeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	UpgradeWidget->GenerateOptions(AvailableUpgrades);
}

void AMLHUD::ShowLostWidget()
{
	CreateAndAddWidget<UUserWidget>(LostWidgetClass);
}

void AMLHUD::ShowWonWidget()
{
	CreateAndAddWidget<UUserWidget>(WonWidgetClass);
}

void AMLHUD::ShowAllWidgets()
{
	if (!MainWidget)
	{
		return;
	}

	MainWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMLHUD::ShowPauseWidget()
{
	if (!PauseWidget || !PauseWidget->IsInViewport())
	{
		PauseWidget = CreateAndAddWidget<UUserWidget>(PauseWidgetClass);
	}
	else if (PauseWidget && PauseWidget->IsInViewport())
	{
		PauseWidget->RemoveFromParent();
	}
}

void AMLHUD::HideAllWidgets()
{
	if (!MainWidget)
	{
		return;
	}

	MainWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AMLHUD::ChangeStyle(bool bForward)
{
	if (!MainWidget)
	{
		return;
	}

	MainWidget->ChangeStyle(bForward);
}

void AMLHUD::UpdateMagazine(TArray<EElements> Magazine)
{
	MainWidget->UpdateMagazine(Magazine);
}
