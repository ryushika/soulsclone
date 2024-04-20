
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Teleporter.generated.h"

class ACharacter;

DECLARE_DELEGATE_OneParam(FOnTargetReached, FTeleportTarget&);

USTRUCT(BlueprintType)
struct FTeleportTarget
{
	GENERATED_BODY()

public:
	FTeleportTarget() = default;
	FTeleportTarget(class ACharacter* NewTarget, class ACameraActor* NewCamera)
	{
		Target = NewTarget;
		Camera = NewCamera;
	}

	UPROPERTY()
	class ACharacter* Target;

	UPROPERTY()
	ACameraActor* Camera;

	UPROPERTY()
	FVector StartLocation = FVector::ZeroVector;

	UPROPERTY()
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY()
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY()
	float Alpha = 0.f;

	UPROPERTY()
	float InterpSpeed = 0.f;

	bool bMarkedForDeletion = false;

	FOnTargetReached OnTargetReached;

	bool operator == (const FTeleportTarget& Other) const
	{
		return Target == Other.Target;
	}
};

USTRUCT(BlueprintType)
struct FMoveTargetData
{
	GENERATED_BODY()

public:
	FMoveTargetData() = default;
	FMoveTargetData(const FVector& NewLocation, const FRotator& NewRotation = FRotator::ZeroRotator)
		: Location(NewLocation)
		, Rotation(NewRotation)
	{
	}

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

UCLASS()
class MEMORYLAKE_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	ATeleporter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* MyCoolFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraComponent* VFXComp;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

//  Functions

public:
	void InitializePair(class ATeleporter* OriginalTeleporter);

	FORCEINLINE FVector GetSpawnPointLocation() const;
	FORCEINLINE FRotator GetSpawnPointRotation() const;

protected:
	void ShowWidget(APlayerController* OwningPlayer);
	void HideWidget();

	UFUNCTION()
	void OnBoxBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
				int32 OtherBodyIndex);

	void BeginTeleporting(class ACharacter* TeleportTarget);

	ACameraActor* CreateCameraAndSetView(ACharacter* TeleportTarget);

	void MoveCameraTo_Timed(FTeleportTarget& TeleportTarget, const FMoveTargetData& MoveTargetData, float Time);
	void MoveCameraTo_Speed(FTeleportTarget& TeleportTarget, const FMoveTargetData& MoveTargetData, float Speed);

	void MoveCameraTo_Worker(float DeltaTime);

	FRotator InterpRotation(FTeleportTarget& TeleportTarget);

	// Called when the teleport sequence (moving the camera)
	// has reached location above its spawn point
	UFUNCTION()
	void FirstStageComplete(FTeleportTarget& TeleportTarget);
	// Called when the teleport sequence (moving the camera)
	// has reached location above the pair's spawn point
	UFUNCTION()
	void SecondStageComplete(FTeleportTarget& TeleportTarget);
	// Called when the teleport sequence (moving the camera)
	// has reached the pair's spawn point
	UFUNCTION()
	void ThirdStageComplete(FTeleportTarget& TeleportTarget);

	void TeleportPlayer(ACharacter* Player, const FMoveTargetData& TargetData);

private:

//  Variables

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ATeleporter> PairTeleporter;

	TObjectPtr<class UBoxComponent> GetBoxComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	TArray<AActor*> IgnoreTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	float ZOffset = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	float TimeToGoUp = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	float SpeedToReachPair = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	float TimeToGoDown = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	TObjectPtr<class USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	TObjectPtr<class UArrowComponent> SpawnPoint;

	UPROPERTY(VisibleAnywhere)
	TArray<FTeleportTarget> CurrentTeleportTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> ControlsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* ControlsWidget;

private:
};
