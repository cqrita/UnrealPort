// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Components/CBehaviorComponent.h"
#include "CBTDecorator_Behavior.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API UCBTDecorator_Behavior : public UBTDecorator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EBehaviorType Behavior;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
