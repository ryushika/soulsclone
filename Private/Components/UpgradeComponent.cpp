// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UpgradeComponent.h"

UUpgradeComponent::UUpgradeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();
    if (UpgradeDataTable)
    {
        RowNames = UpgradeDataTable->GetRowNames();
    }
}

// Called every frame
void UUpgradeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<FUpgradeStruct> UUpgradeComponent::GetOptions() const
{
    TArray<FUpgradeStruct> AvailableOptions;
    FString ContextString;

    for (auto& name : RowNames)
    {
        FUpgradeStruct* Row = UpgradeDataTable->FindRow<FUpgradeStruct>(name, ContextString);
        if (Row)
        {
            FString RowPrerequisiteStr = Row->Prerequisite.ToString();

            if (RowPrerequisiteStr.IsEmpty() || 
                CollectedUpgrades.ContainsByPredicate([&RowPrerequisiteStr](const FText& Name) 
                    { 
                        return Name.ToString() == RowPrerequisiteStr;
                    }
                ))
            {
                bool bShouldAdd = true;
                for (auto& upgrade : CollectedUpgrades)
                {
                    if (upgrade.ToString() == Row->Name.ToString())
                    {
                        bShouldAdd = false;
                    }
                }
                if (bShouldAdd)
                {
                    AvailableOptions.Add(*Row);
                }
              
            }
        }
    }

    return AvailableOptions;
}

void UUpgradeComponent::Upgrade(FUpgradeStruct UpgradeStruct)
{
    CurrentUpgradeStruct = UpgradeStruct;
    
    CollectedUpgrades.Add(UpgradeStruct.Name);
}

bool UUpgradeComponent::QueryUpgrade(FText Upgrade)
{
    return CollectedUpgrades.ContainsByPredicate([&Upgrade](const FText& Item) 
        {
        return Item.EqualTo(Upgrade, ETextComparisonLevel::Default);
        }
    );
}

bool UUpgradeComponent::CheckPrerequisite(FText Requirement)
{
    if (Requirement.IsEmpty())
    {
        return true;
    }

    return CollectedUpgrades.ContainsByPredicate([&Requirement](const FText& Item)
        {
            return Item.EqualTo(Requirement, ETextComparisonLevel::Default);
        }
    );
}

