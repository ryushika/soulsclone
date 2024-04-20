// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/InteractInterface.h"

#include "Shopkeeper.generated.h"

USTRUCT(BlueprintType)
struct FShopperData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerCharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AMemoryLakePlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UUserWidget> ShopWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UUserWidget> ControlsWidget;

	bool operator == (const FShopperData& Other) const
	{
		return Character == Other.Character;
	}
};

UCLASS()
class MEMORYLAKE_API AShopkeeper : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AShopkeeper();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StopFocus(FShopperData& Shopper);
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void StopFocus(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact(APlayerCharacter* Interactor);
	virtual void Interact_Implementation(APlayerCharacter* Interactor);

	void CreateShopWidget(FShopperData& Shopper);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
	UUserWidget* CreateShopWidget_Worker(APlayerController* OwningPlayer);
	void ShowWidget(FShopperData& Shopper);
	void HideWidget(FShopperData& Shopper);

	// will return -1 if not found
	int32 GetShopperIndex(AActor* ByActor);
	// will return -1 if not found
	int32 GetShopperIndex(AController* ByController);
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UBoxComponent* ReactTriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> ControlsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> ShopWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FShopperData> Shoppers;
};
