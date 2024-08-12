// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CEquipment.h"
#include "CWeapon.generated.h"

class ACCharacterBase;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UCWeaponDataAsset;
class UCEquipmentDataAsset;
/**
 * 
 */
UCLASS()
class UNREALP_API ACWeapon : public ACEquipment
{
	GENERATED_BODY()
public:
	ACWeapon();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ACEquipment> EquipmentSet;
public:
	UFUNCTION()
	virtual void SetEquipmentSet(ACEquipment* Equipment);
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCWeaponDataAsset* GetWeaponDataAsset() const;
};
