// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility_Block.h"
#include "Components/CCombatComponent.h"
#include "Interface/ICombatInterface.h"
#include "Items/CWeapon.h"
#include "Items/CWeaponDataAsset.h"
void UCGameplayAbility_Block::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(ActorInfo->AvatarActor);
	OwnerCombatComponent = CombatInterface->GetCombatComponent();
	if (OwnerCombatComponent == nullptr)
	{
		return;
	}
	OwnerCombatComponent->OnBlockEnd.AddDynamic(this, &UCGameplayAbility_Block::BlockEnd);
}

UAnimMontage* UCGameplayAbility_Block::GetBlockMontage() const
{
	ACWeapon* weapon = Cast<ACWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	return weapon->GetWeaponDataAsset()->BlockMontage;
}
