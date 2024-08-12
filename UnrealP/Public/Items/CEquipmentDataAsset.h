// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CEquipmentDataAsset.generated.h"
class UBlendSpace;
class ACEquipment;
class UAnimMontage;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREALP_API UCEquipmentDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> EquipMontage;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> EquipAbility;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> UnequipMontage;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> UnequipAbility;
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;
};
