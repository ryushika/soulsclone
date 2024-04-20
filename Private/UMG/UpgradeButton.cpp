// Fill out your copyright notice in the DescriptionStr page of Project Settings.


#include "UMG/UpgradeButton.h"

void UUpgradeButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUpgradeButton::InitButton(FUpgradeStruct UpgradeStruct)
{
	FString DescriptionStr;

	if (Name)
	{
		Name->SetText(FText::FromString(FString::Printf(TEXT("%s"), *UpgradeStruct.Name.ToString())));
		UpgradeName = UpgradeStruct.Name;
		ButtonUpgradeStruct = UpgradeStruct;
	}
	if (!UpgradeStruct.Description.IsEmptyOrWhitespace())
	{
		Description->SetText(UpgradeStruct.Description);
	}
	if (UpgradeStruct.Level != 0)
	{
		Level->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), UpgradeStruct.Level)));
	}

	if (UpgradeStruct.Icon)
	{
		Icon->SetBrushFromTexture(UpgradeStruct.Icon, true);
	}
	
	if (UpgradeStruct.FireDamage > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Fire Damage * %.2f\n"), UpgradeStruct.FireDamage);
	}
	if (UpgradeStruct.FirePeriodDamage > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Fire Period Damage * %.2f\n"), UpgradeStruct.FirePeriodDamage);
	}
	if (UpgradeStruct.FirePeriodDuration > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Fire Period Duration +.2f sec\n"), UpgradeStruct.FirePeriodDuration);
	}
	if (UpgradeStruct.WaterDamage > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Water Damage * %.2f\n"), UpgradeStruct.WaterDamage);
	}
	if (UpgradeStruct.WaterSlowPower > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Water Slow Power * %.2f\n"), UpgradeStruct.WaterSlowPower);
	}
	if (UpgradeStruct.WaterSlowDuration > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Water Slow Duration + %.2f sec\n"), UpgradeStruct.WaterSlowDuration);
	}
	if (UpgradeStruct.EarthDamage > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Earth Damage * %.2f\n"), UpgradeStruct.EarthDamage);
	}
	if (UpgradeStruct.EarthStunDuration > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Earth Stun Duration + %.2f sec\n"), UpgradeStruct.EarthStunDuration);
	}
	if (UpgradeStruct.WindDamage > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Wind Damage * %.2f\n"), UpgradeStruct.WindDamage);
	}
	if (UpgradeStruct.WindKnockBack > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Wind Knock Back * %.2f\n"), UpgradeStruct.WindKnockBack);
	}

	if (UpgradeStruct.MovementSpeed > 1.0) {
		DescriptionStr += FString::Printf(TEXT("Movement Speed * %.2f\n"), UpgradeStruct.MovementSpeed);
	}

	if (!UpgradeStruct.Description.IsEmpty()) {
		Description->SetText(UpgradeStruct.Description);
	}
	else 
	{
		Description->SetText(FText::FromString(DescriptionStr));
	}

	Button->OnClicked.AddUniqueDynamic(this, &UUpgradeButton::OnClicked);
}

void UUpgradeButton::OnClicked()
{
	OnUpgradeButtonClicked.Broadcast(ButtonUpgradeStruct);
}