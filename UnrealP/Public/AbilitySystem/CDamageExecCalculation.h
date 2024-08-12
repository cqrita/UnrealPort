// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CDamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API UCDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UCDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
