// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthWidgetComponent.generated.h"

UCLASS()
class MEMORYLAKE_API UHealthWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void UpdateHealthBar(float Percent);
};
