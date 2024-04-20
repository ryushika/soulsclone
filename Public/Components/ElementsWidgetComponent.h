// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "../MemoryLake.h"
#include "ElementsWidgetComponent.generated.h"

class UChoosenElementsWidget;

UCLASS()
class MEMORYLAKE_API UElementsWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void UpdateDisplayedElements(TArray<EElements> const& Elements);

private:
	UChoosenElementsWidget* ElementsWidget;
};
