// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/BulletWidget.h"

void UBulletWidget::SetColor()
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("coloring called !"));
    switch (BulletElement)
    {
    case EElements::WATER:
        BulletBorder->SetBrushColor(FLinearColor(0.0f, 0.5f, 1.0f)); 
        break;
    case EElements::FIRE:
        BulletBorder->SetBrushColor(FLinearColor(1.0f, 0.0f, 0.0f)); 
        break;
    case EElements::AIR:
        BulletBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f));
        break;
    case EElements::EARTH:
        BulletBorder->SetBrushColor(FLinearColor(0.5f, 1.5f, 0.2f)); 
        break;
    case EElements::LIGHTNING:
        BulletBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 0.0f)); 
        break;
    default:
        BulletBorder->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f));
        break;
    }
}
