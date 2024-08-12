// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CEquipmentDataAsset.h"
#include "CWeaponDataAsset.generated.h"
class UBlendSpace;
class ACEquipment;
class UAnimMontage;
class UGameplayAbility;
class UMaterialInstance;
/**
 * 
 */
UCLASS()
class UNREALP_API UCWeaponDataAsset : public UCEquipmentDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBlendSpace> WeaponBlendSpace;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACEquipment> EquipmentSetClass;
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> GrantedAblities;
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> BaseAttackMontages;
	UPROPERTY(EditDefaultsOnly)
	float BaseAttackSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UAnimMontage> BlockMontage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USoundBase> HitSound;
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> SpecialAbility;
};
