// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CGameplayAbility.h"
#include "AbilitySystem/FCGlobalTags.h"
#include "CGameplayAbility_Damage.generated.h"

class UCCombatComponent;
class AActor;
/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility_Damage : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCCombatComponent> OwnerCombatComponent;
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeDamageSpecHandle();
	UFUNCTION(BlueprintCallable)
	void CauseDamageWithLocation(AActor* TargetActor,FHitResult HitResult);
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeDamageSpecHandleLocation(FHitResult HitResult);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FGameplayTag DamageTag= FCGlobalTags::Get().DamageTag;
protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(TSubclassOf<ACProjectile> ProjectileClass, FVector ProjectileStartLocation, FVector ProjectileTargetLocation, bool bOverridePitch = false, float PitchOverride = 0.f);

};
