

#include "Components/CursorWidgetComponent.h"
#include "../../MemoryLakePlayerController.h"
#include "Kismet/KismetMathLibrary.h"

void UCursorWidgetComponent::Initialize(APlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;
}

void UCursorWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldLookAtCursor && bCanMoveCursor)
	{
		LookAtCursor();
	}
}

void UCursorWidgetComponent::MouseMove(const FInputActionValue& Value)
{
	FVector2D MouseXY = Value.Get<FVector2D>();

	if (bCanMoveCursor)
	{
		const FVector WorldDecalLocation = GetComponentLocation();

		FVector2D ScreenLocation;
		PlayerController->ProjectWorldLocationToScreen(WorldDecalLocation, ScreenLocation);

		ScreenLocation.X += (MouseXY.X * Sensitivity);
		ScreenLocation.Y += (MouseXY.Y * Sensitivity);
		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		ScreenLocation.X = FMath::Clamp(ScreenLocation.X, 0.f, ViewportSizeX);
		ScreenLocation.Y = FMath::Clamp(ScreenLocation.Y, 0.f, ViewportSizeY);

		FVector NewScreenLoc = DeprojectScreenToWorld(ScreenLocation);
		SetWorldLocation(NewScreenLoc);
	}
}

void UCursorWidgetComponent::LookAtCursor()
{
	if (PlayerController)
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector CursorLocation = GetComponentLocation();

		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, CursorLocation);

		AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(PlayerController);
		if (MLController && !MLController->bOnLadder)
		{
			PlayerController->SetControlRotation(NewRotation);
		}	
	}
}

bool UCursorWidgetComponent::SetCursorScreenLocFromWorld(const FVector& Location, bool bForceSet)
{
	if (PlayerController)
	{
		FVector2D ScreenLocation;
		PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);

		int32 ViewportSizeX, ViewportSizeY;
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

		int32 X = ScreenLocation.X;
		int32 Y = ScreenLocation.Y;

		int32 Padding = 20;

		bool bSuccess = X >= Padding && X <= ViewportSizeX - Padding && Y >= Padding && Y <= ViewportSizeY - Padding;

		if (bSuccess)
		{
			PlayerController->SetMouseLocation(X, Y);
		}
		else if (bForceSet)
		{
			X = FMath::Clamp(X, Padding, ViewportSizeX - Padding);
			Y = FMath::Clamp(Y, Padding, ViewportSizeY - Padding);

			PlayerController->SetMouseLocation(X, Y);
		}

		return bSuccess;
	}

	return false;
}

FHitResult UCursorWidgetComponent::GetCursorHitResult()
{
	FHitResult CursorHitResult;
	if (PlayerController)
	{
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, CursorHitResult);
	}
	return CursorHitResult;
}

FVector UCursorWidgetComponent::GetCursorWorldPosition()
{
	FVector CursorLocation = FVector::ZeroVector;

	if (PlayerController)
	{
		FHitResult CursorHitResult = GetCursorHitResult();
		CursorLocation = CursorHitResult.Location;
	}

	return CursorLocation;
}

FVector UCursorWidgetComponent::DeprojectScreenToWorld(const FVector2D& ScreenPosition)
{
	FVector Location = FVector::ZeroVector;
	FVector Direction = FVector::ZeroVector;

	if (PlayerController && GetWorld())
	{
		PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, Location, Direction);
		
		FHitResult HitResult;        
		FVector End = Location + (Direction * 2000.f);
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End, ECC_Visibility);

		if (bHit)
		{
			Location = HitResult.Location;
		}
		else
		{
			Location = End;
		}
	}

	return Location;
}
