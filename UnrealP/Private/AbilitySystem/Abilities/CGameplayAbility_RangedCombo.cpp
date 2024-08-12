// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility_RangedCombo.h"
#include "Components/CCombatComponent.h"

void UCGameplayAbility_RangedCombo::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if (OwnerCombatComponent == nullptr)
	{
		return;
	}
	OwnerCombatComponent->OnComboCall.AddDynamic(this, &UCGameplayAbility_RangedCombo::ComboCall);
	OwnerCombatComponent->OnProjectileFire.AddDynamic(this, &UCGameplayAbility_RangedCombo::ProjectileFire);

}
