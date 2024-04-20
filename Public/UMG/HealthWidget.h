// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UProgressBar;

UCLASS()
class MEMORYLAKE_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealthBar(float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
};
