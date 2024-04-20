// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/UpgradeWidget.h"
#include "../MemoryLakePlayerController.h"

void UUpgradeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UpgradeButton1->OnUpgradeButtonClicked.AddDynamic(this, &UUpgradeWidget::HandleUpgradeEvent);
	UpgradeButton2->OnUpgradeButtonClicked.AddDynamic(this, &UUpgradeWidget::HandleUpgradeEvent);
	UpgradeButton3->OnUpgradeButtonClicked.AddDynamic(this, &UUpgradeWidget::HandleUpgradeEvent);
	bUpgradeClicked = false;

	OnVisibilityChanged.AddDynamic(this, &UUpgradeWidget::VisibilityChanged);
}

void UUpgradeWidget::GenerateOptions(TArray<FUpgradeStruct> AvailableUpgrades)
{
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();

	 for (int i = AvailableUpgrades.Num() - 1; i >= 1; i--)
	 {
       int32 j = RandomStream.RandRange(0, i);
       Swap(AvailableUpgrades[i], AvailableUpgrades[j]);
	 }
	 if (UpgradeButton1 && AvailableUpgrades.Num() > 0)
	 {
		UpgradeButton1->InitButton(AvailableUpgrades[0]);
	 }
	 else
	 {
		UpgradeButton1->RemoveFromParent();
		RemoveFromParent();
	 }
	 if (UpgradeButton2 && AvailableUpgrades.Num() > 1)
	 {
		 UpgradeButton2->InitButton(AvailableUpgrades[1]);
	 }
	 else 
	 {
	 	UpgradeButton2->RemoveFromParent();
	 }
	 if (UpgradeButton3 && AvailableUpgrades.Num() > 2)
	 {
	 	UpgradeButton3->InitButton(AvailableUpgrades[2]);
	 }
	 else
	 {
	 	UpgradeButton3->RemoveFromParent();
	 }
}

void UUpgradeWidget::HandleUpgradeEvent(FUpgradeStruct UpgradeStruct)
{
	if (!bUpgradeClicked)
	{
		AMemoryLakePlayerController* OwningController = GetOwningPlayer<AMemoryLakePlayerController>();
		if (!OwningController)
		{
			return;
		}
		OwningController->HandleUpgradeChoosing(UpgradeStruct);
		HideUpgrades();
		bUpgradeClicked = true;
	}
}

void UUpgradeWidget::VisibilityChanged(ESlateVisibility InVisibility)
{

	if (InVisibility != ESlateVisibility::Collapsed)
	{
		if (Player)
		{
			Player->StopCursorWidget();
		}
		GetOwningPlayer()->SetShowMouseCursor(true);
		GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	}
}
