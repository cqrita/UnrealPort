// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CInputDataAsset.generated.h"

class UGameplayAbility;
class UInputAction;
/**
 * 
 */
UCLASS()
class UNREALP_API UCInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<TSubclassOf<UGameplayAbility>, UInputAction*> ActionInput;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int, UInputAction*> EquipmentInput;
};
