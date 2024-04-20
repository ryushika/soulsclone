// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthWidgetComponent.h"
#include "UMG/HealthWidget.h"

void UHealthWidgetComponent::UpdateHealthBar(float Percent)
{
	UHealthWidget* HealthWidget = Cast<UHealthWidget>(GetWidget());
	if (!HealthWidget)
	{
		return;
	}

	HealthWidget->UpdateHealthBar(Percent);
}