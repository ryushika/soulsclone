// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeButton.h"
#include "Characters/PlayerCharacter.h"
#include "Components/UpgradeComponent.h"
#include "DataTypes.h"
#include "UpgradeWidget.generated.h"

UCLASS()
class MEMORYLAKE_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* UpgradeDataTable;

	UFUNCTION(BlueprintCallable)
	void GenerateOptions(TArray<FUpgradeStruct> AvailableUpgrades);

	UFUNCTION()
	void HandleUpgradeEvent(FUpgradeStruct UpgradeStruct);

	UFUNCTION()
	void VisibilityChanged(ESlateVisibility InVisibility);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowUpgrades();

	UFUNCTION(BlueprintImplementableEvent)
	void HideUpgrades();
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUpgradeButton> UpgradeButton1;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUpgradeButton> UpgradeButton2;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUpgradeButton> UpgradeButton3;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerCharacter> Player;

	UPROPERTY()
	bool bUpgradeClicked;

};
