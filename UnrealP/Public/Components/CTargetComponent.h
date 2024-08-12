// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"

class ACharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Distance=1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Forward = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius=100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::ForOneFrame;
	UPROPERTY()
	TObjectPtr<ACharacter> Target;
private:
	TObjectPtr<ACharacter> Owner;
	TObjectPtr<ACharacter> TempTarget;
	float Closest;
		
};
