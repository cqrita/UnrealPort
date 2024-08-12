// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CGameplayAbility.generated.h"

class ACProjectile;
class ACharacter;
/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCGameplayAbility();
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	ACharacter* OwnerCharacter;
protected:
	UFUNCTION(BlueprintCallable)
	FTransform GetWarpTransform(float Distance, float WarpDistance,bool& bOutOfDistance,bool& bOnlyRotate);
	UFUNCTION(BlueprintCallable)
	ACharacter* GetEnemyTarget();
};
