// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CWeapon.h"
#include "CMeleeWeapon.generated.h"

class UFieldSystemComponent;
class URadialVector;
class URadialFalloff;
/**
 * 
 */
UCLASS()
class UNREALP_API ACMeleeWeapon : public ACWeapon
{
	GENERATED_BODY()
public:
	ACMeleeWeapon();
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCapsuleComponent*> CapsuleArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFieldSystemComponent> FieldSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URadialVector> RadialVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URadialFalloff> RadialFalloff;
	virtual void SetEquipmentSet(ACEquipment* Equipment) override;
};
