// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

class UCStatusDataAsset;


UENUM(BlueprintType)
enum class EHitReactDirection : uint8
{
	ENone,
	ELeft,
	EFront,
	ERight,
	EBack
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	bool bIsAlive=true;
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsAlive() const { return bIsAlive; }
	UFUNCTION(BlueprintCallable)
	void SetIsAlive(bool IsAlive);
public:
	UFUNCTION(BlueprintCallable)
	EHitReactDirection GetHitReactDirection(const FVector& ImpactPoint) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCStatusDataAsset> StatusDataAsset;
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCStatusDataAsset* GetStatusDataAsset() const { return StatusDataAsset; }
};
