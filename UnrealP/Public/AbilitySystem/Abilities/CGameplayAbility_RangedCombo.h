// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CGameplayAbility_Damage.h"
#include "Interface/IComboInterface.h"
#include "CGameplayAbility_RangedCombo.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility_RangedCombo : public UCGameplayAbility_Damage ,public IIComboInterface
{
	GENERATED_BODY()
public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bComboFlag = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentComboNum = 0;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ComboCall();
	UFUNCTION(BlueprintImplementableEvent)
	void ProjectileFire();

};
