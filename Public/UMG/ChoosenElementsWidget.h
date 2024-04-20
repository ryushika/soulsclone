// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MemoryLake.h"
#include "ChoosenElementsWidget.generated.h"

class UImage;

UCLASS()
class MEMORYLAKE_API UChoosenElementsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void UpdateChoosenElements(TArray<EElements> const& Elements);

protected:
	TStaticArray<UImage*, 3> ChoosenElements;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Images")
	TMap<EElements, UTexture2D*> ElementImageMap;

	UPROPERTY(meta = (BindWidget))
	UImage* FirstImage;

	UPROPERTY(meta = (BindWidget))
	UImage* SecondImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ThirdImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Images")
	UTexture2D* EmptyImage;

private:
	void ClearElements();
};
