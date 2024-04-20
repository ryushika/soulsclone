// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MLGameInstance.generated.h"

UCLASS()
class MEMORYLAKE_API UMLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;

	UFUNCTION()
	void BeginLoadingScreen(const FString& InMapName);
	
	FORCEINLINE FName GetMainMenuMapName() const { return MainMenuMapName; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Maps")
	FName MainMenuMapName;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LoadingScreenClass;

};
