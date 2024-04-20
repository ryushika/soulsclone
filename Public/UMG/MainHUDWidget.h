// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "UMG/BulletWidget.h"
#include "MemoryLake.h"
#include "MainHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MEMORYLAKE_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealth(float HealthPercent);
	void SetFocus(int32 FocusValue);
	void ChangeStyle(bool bForward);
	void UpdateMagazine(TArray<EElements> Magazine);

	UPROPERTY()
	UWidgetSwitcher* CurrentStyle;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FocusValueText;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* BulletContainer;	

	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet1;
	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet2;
	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet3;
	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet4;
	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet5;
	UPROPERTY(meta = (BindWidget))
	UBulletWidget* Bullet6;

};
