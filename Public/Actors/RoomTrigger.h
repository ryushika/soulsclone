// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTrigger.generated.h"

class UBoxComponent;
class AEnemyCharacter;
class AEnemyAIController;
class UNavigationInvokerComponent;

USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEnemyAIController> ControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemiesCountMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EnemiesCountMin;
};

UCLASS()
class MEMORYLAKE_API ARoomTrigger : public AActor
{
	GENERATED_BODY()

public:
	ARoomTrigger();

	FORCEINLINE TArray<FEnemySpawnInfo> GetEnemiesSpawnInfo() { return EnemiesSpawnInfo; }

	FVector GetRandomPointInRoom();
	FORCEINLINE bool IsLastRoom() const { return bLastRoom; }
	FORCEINLINE USoundBase* GetMusicToPlay() const { return MusicToPlay; }

	UPROPERTY(VisibleAnywhere)
	int32 SpawnedEnemies = 0;

protected:
	void BeginPlay();

	UFUNCTION()
	void OnWallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TArray<UBoxComponent*> Walls;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Wall0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Wall1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Wall2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Wall3;

	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (AllowPrivateAccess = "true"))
	UNavigationInvokerComponent* NavigationInvoker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemies", meta = (AllowPrivateAccess = "true"))
	TArray<FEnemySpawnInfo> EnemiesSpawnInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	USoundBase* MusicToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	bool bLastRoom = false;

private:
	void CreateWalls();
	void SetupWalls();
};
