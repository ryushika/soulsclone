// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/ChoosenElementsWidget.h"
#include "Components/Image.h"

bool UChoosenElementsWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return Success;

	if (FirstImage && SecondImage && ThirdImage)
	{
		ChoosenElements[0] = FirstImage;
		ChoosenElements[1] = SecondImage;
		ChoosenElements[2] = ThirdImage;
		ClearElements();
		return true;
	}

	return false;
}

void UChoosenElementsWidget::UpdateChoosenElements(TArray<EElements> const& Elements)
{
	ClearElements();
	if (Elements.IsEmpty())
	{
		return;
	}
	for (int i = 0; i < Elements.Num(); i++)
	{
		UTexture2D* ElementImage = *ElementImageMap.Find(Elements[i]);
		if (!ElementImage || !ChoosenElements[i]) continue;

		ChoosenElements[i]->SetBrushFromTexture(ElementImage);
	}
}

void UChoosenElementsWidget::ClearElements()
{
	for (auto ChoosenElement : ChoosenElements)
	{
		if (!ChoosenElement) continue;

		ChoosenElement->SetBrushFromTexture(EmptyImage);
	}
}