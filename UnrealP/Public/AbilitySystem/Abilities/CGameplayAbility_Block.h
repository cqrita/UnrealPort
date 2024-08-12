// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CGameplayAbility.h"
#include "CGameplayAbility_Block.generated.h"

class UCCombatComponent;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility_Block : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCCombatComponent> OwnerCombatComponent;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void BlockEnd();
public:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetBlockMontage() const;
};
