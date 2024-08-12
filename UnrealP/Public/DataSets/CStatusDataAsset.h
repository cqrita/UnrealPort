// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CStatusDataAsset.generated.h"
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class UNREALP_API UCStatusDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> OnHitFrontMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> OnHitLeftMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> OnHitRightMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> OnHitBackMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> OnDeathMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> OnDeathXP;
};
