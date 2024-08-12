// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility_Damage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/Actors/CProjectile.h"
#include "Components/CCombatComponent.h"
#include "Items/CWeapon.h"
#include "Interface/ICombatInterface.h"

void UCGameplayAbility_Damage::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(ActorInfo->AvatarActor);
	OwnerCombatComponent = CombatInterface->GetCombatComponent();
}

void UCGameplayAbility_Damage::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeDamageSpecHandle();
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FGameplayEffectSpecHandle UCGameplayAbility_Damage::MakeDamageSpecHandle()
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageTag, ScaledDamage);
	if (OwnerCombatComponent->GetCurrentWeapon())
	{
		DamageSpecHandle.Data.Get()->GetContext().AddSourceObject(OwnerCombatComponent->GetCurrentWeapon());
	}
	return DamageSpecHandle;
}

void UCGameplayAbility_Damage::CauseDamageWithLocation(AActor* TargetActor, FHitResult HitResult)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeDamageSpecHandleLocation(HitResult);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FGameplayEffectSpecHandle UCGameplayAbility_Damage::MakeDamageSpecHandleLocation(FHitResult HitResult)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageTag, ScaledDamage);
	if (OwnerCombatComponent->GetCurrentWeapon())
	{
		DamageSpecHandle.Data.Get()->GetContext().AddSourceObject(OwnerCombatComponent->GetCurrentWeapon());
	}
	UAbilitySystemBlueprintLibrary::EffectContextAddHitResult(DamageSpecHandle.Data.Get()->GetEffectContext(), HitResult, true);
	return DamageSpecHandle;
}

void UCGameplayAbility_Damage::SpawnProjectile(TSubclassOf<ACProjectile> ProjectileClass, FVector ProjectileStartLocation, FVector ProjectileTargetLocation, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FRotator Rotation = (ProjectileTargetLocation - ProjectileStartLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ProjectileStartLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ACProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->DamageSpec = MakeDamageSpecHandle();
	Projectile->FinishSpawning(SpawnTransform);
}
