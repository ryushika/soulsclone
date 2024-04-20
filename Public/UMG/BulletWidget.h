// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MemoryLake.h"
#include "Components/Border.h"
#include "BulletWidget.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYLAKE_API UBulletWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UBorder* BulletBorder;

	UPROPERTY(BlueprintReadWrite)
	EElements BulletElement;
	
	UFUNCTION(BlueprintCallable)
	void SetColor();
};
