// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataTypes.h"
#include "Components/ActorComponent.h"
#include "UpgradeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORYLAKE_API UUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradeComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	UDataTable* UpgradeDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FUpgradeStruct> AllUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FText> CollectedUpgrades;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FName> RowNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUpgradeStruct CurrentUpgradeStruct;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	TArray<FUpgradeStruct> GetOptions() const;

	UFUNCTION(BlueprintCallable)
	void Upgrade(FUpgradeStruct UpgradeStruct);

	UFUNCTION(BlueprintCallable)
	bool QueryUpgrade(FText Upgrade);

	UFUNCTION(BlueprintCallable)
	bool CheckPrerequisite(FText Requiremen);
};
