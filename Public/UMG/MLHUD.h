// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../MemoryLake.h"
#include "MLHUD.generated.h"

class UChoosenElementsWidget;
class UMainHUDWidget;
class UUpgradeComponent;
class UUpgradeWidget;

UCLASS()
class MEMORYLAKE_API AMLHUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetHealth(float HealthPercent);
	void SetFocusValue(int32 FocusValue);

	void ShowUpgrades(UUpgradeComponent const* OwnerUpgradeComponent);
	void ShowLostWidget();
	void ShowWonWidget();

	void ShowAllWidgets();

	void ShowPauseWidget();
	void HideAllWidgets();
	
	void ChangeStyle(bool bForward);
	void UpdateMagazine(TArray<EElements> Magazine);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainHUDWidget> MainWidgetClass;

	UPROPERTY()
	UMainHUDWidget* MainWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUpgradeWidget> UpgradeWidgetClass;

	UPROPERTY()
	UUpgradeWidget* UpgradeWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LostWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> WonWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PauseWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> PauseWidgetClass;

private:
	virtual void PostInitializeComponents() override;

	template <typename T>
	T* CreateAndAddWidget(TSubclassOf<UUserWidget> WidgetClass);
};
