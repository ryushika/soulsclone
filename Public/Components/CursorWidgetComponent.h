// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InputActionValue.h"

#include "CursorWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class MEMORYLAKE_API UCursorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
//  Functions

public:
	void Initialize(class APlayerController* NewPlayerController);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MouseMove(const FInputActionValue& Value);

protected:
	void LookAtCursor();

	bool SetCursorScreenLocFromWorld(const FVector& Location, bool bForceSet);

	FHitResult GetCursorHitResult();
	FVector GetCursorWorldPosition();

	FVector DeprojectScreenToWorld(const FVector2D& ScreenPosition);

private:

//  Variables

public:

protected: 
	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	bool bShouldLookAtCursor = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	bool bCanMoveCursor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	float Sensitivity = 100.f;

private:
};
