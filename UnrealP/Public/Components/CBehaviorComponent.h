// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	EWait, EApproach, EAction, EPatrol, EOnHit, EAvoid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBehaviorTypeChanged, EBehaviorType, InPrevType, EBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCBehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	EBehaviorType Behavior = EBehaviorType::EWait;
public:
	UFUNCTION(BlueprintPure)
	EBehaviorType GetBehavior();
	UFUNCTION(BlueprintCallable)
	void SetBehavior(EBehaviorType InBehavior);
public:
	UPROPERTY(BlueprintAssignable)
	FBehaviorTypeChanged OnBehaviorTypeChanged;
};
