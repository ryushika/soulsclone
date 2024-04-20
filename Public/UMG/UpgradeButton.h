// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "DataTypes.h"
#include "UpgradeButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeButtonClicked, FUpgradeStruct, UpgradeStruct);
/**
 * 
 */

UCLASS()
class MEMORYLAKE_API UUpgradeButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void InitButton(FUpgradeStruct UpgradeStruct);

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FOnUpgradeButtonClicked OnUpgradeButtonClicked;
protected:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UButton> Button;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UImage> Icon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr <UTextBlock> Name;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTextBlock> Description;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTextBlock> Level;

	UPROPERTY(BlueprintReadWrite)
	FText UpgradeName;

	UPROPERTY(BlueprintReadWrite)
	FUpgradeStruct ButtonUpgradeStruct;

};
