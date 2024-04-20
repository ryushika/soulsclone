// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ElementsWidgetComponent.h"
#include "UMG/ChoosenElementsWidget.h"

void UElementsWidgetComponent::UpdateDisplayedElements(TArray<EElements> const& Elements)
{
	ElementsWidget = ElementsWidget ? ElementsWidget : Cast<UChoosenElementsWidget>(GetWidget());
	if (!ElementsWidget)
	{
		return;
	}
	ElementsWidget->UpdateChoosenElements(Elements);
}