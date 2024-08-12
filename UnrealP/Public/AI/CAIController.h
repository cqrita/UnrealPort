// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/IBehaviorInterface.h"
#include "CAIController.generated.h"

class UAIPerceptionComponent;
class UCBehaviorComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
class UBehaviorTree;
class ACharacter;
class UCStatusComponent;
/**
 * 
 */
UCLASS()
class UNREALP_API ACAIController : public AAIController, public IIBehaviorInterface
{
	GENERATED_BODY()
private:
    TObjectPtr<UCBehaviorComponent> Behavior;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UBehaviorTree> BehaviorTree;
protected:
    virtual void BeginPlay() override;

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

private:
    UFUNCTION()
    void OnPerception(AActor* TargetActor,FAIStimulus Stimulus);
    UFUNCTION()
    void OnPerceptionForgotten(AActor* TargetActor);
public:
    ACAIController(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;

    float GetSightRadius();

private:
    TObjectPtr<UAISenseConfig_Sight> Sight;
    TObjectPtr<UAISenseConfig_Damage> Damage;

protected:
    virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
public:
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
    AActor* Target;
    UPROPERTY()
    TObjectPtr<UCStatusComponent> StatusComponent;
public:
    virtual EBehaviorType GetBehavior() override;
    virtual void SetBehavior(EBehaviorType InBehavior) override;

//smooth rotation
protected:
    FRotator SmoothTargetRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SmoothFocusInterpSpeed = 30.0f;

public:
    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
};
