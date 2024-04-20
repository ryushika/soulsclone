// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicCameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORYLAKE_API UDynamicCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDynamicCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector FindCameraPosition();
	float FindCameraBoomDistacne();

private:
	FVector CameraPosition;
	class APlayerCharacter* Owner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddTarget(AActor* NewTarget);
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void RemoveTarget(AActor* NewTarget);
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ClearTargets();
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraPosition();
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ScanForEnemies();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> CameraTargets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float ScanDistance = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MinBoomLength = 800;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MaxBoomLength = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BoomLenghtMult = 2.3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bActiveDynamicCamera = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BoomLength;
};
