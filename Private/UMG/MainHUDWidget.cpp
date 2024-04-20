// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/MainHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainHUDWidget::SetHealth(float HealthPercent)
{
	if (!HealthBar)
	{
		return;
	}

	HealthBar->SetPercent(HealthPercent);
}

void UMainHUDWidget::SetFocus(int32 FocusValue)
{
	if (!FocusValueText)
	{
		return;
	}

	FocusValueText->SetText(FText::FromString(FString::FromInt(FocusValue)));
}

void UMainHUDWidget::ChangeStyle(bool bForward)
{
	int32 CurrentIndex = CurrentStyle->GetActiveWidgetIndex();
	int32 TotalWidgets = CurrentStyle->GetNumWidgets();
	int32 NextIndex;
	if (bForward)
	{
		NextIndex = (CurrentIndex + 1) % TotalWidgets;
	}
	else
	{
		if (CurrentIndex == 0)
		{
			NextIndex = TotalWidgets - 1; // Go to the last index
		}
		else
		{
			NextIndex = (CurrentIndex - 1) % TotalWidgets;
		}
	}

	CurrentStyle->SetActiveWidgetIndex(NextIndex);
}

void UMainHUDWidget::UpdateMagazine(TArray<EElements> Magazine)
{
	if (Magazine.Num() == 0)
	{
		return;
	}

	for (int32 i = 0; i < Magazine.Num() && i < 6; ++i) 
	{
		switch (i)
		{
		case 0:
			Bullet1->BulletElement = Magazine[i];
			Bullet1->SetColor();
			break;
		case 1:
			Bullet2->BulletElement = Magazine[i];
			Bullet2->SetColor();
			break;
		case 2:
			Bullet3->BulletElement = Magazine[i];
			Bullet3->SetColor();
			break;
		case 3:
			Bullet4->BulletElement = Magazine[i];
			Bullet4->SetColor();
			break;
		case 4:
			Bullet5->BulletElement = Magazine[i];
			Bullet5->SetColor();
			break;
		case 5:
			Bullet6->BulletElement = Magazine[i];
			Bullet6->SetColor();
			break;
		}
	}
}
