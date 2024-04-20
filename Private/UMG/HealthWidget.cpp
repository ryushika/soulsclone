// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/HealthWidget.h"
#include "Components/ProgressBar.h"

void UHealthWidget::UpdateHealthBar(float Percent)
{
	if (!HealthBar)
	{
		return;
	}

	HealthBar->SetPercent(Percent);
}
