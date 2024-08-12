// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility_Melee.h"
#include "Components/CCombatComponent.h"
#include "Characters/CCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/CMeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Items/CWeaponDataAsset.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "AbilitySystem/CAttributeSet.h"
void UCGameplayAbility_Melee::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if (OwnerCombatComponent == nullptr)
	{
		return;
	}
	OwnerCombatComponent->OnComboCall.AddDynamic(this, &UCGameplayAbility_Melee::ComboCall);
}

bool UCGameplayAbility_Melee::CheckIfHit(AActor* InActor)
{
	if (InActor == GetAvatarActorFromActorInfo())
	{
		return false;
	}
	for (AActor* HitActor : HitArray)
	{
		if (InActor == HitActor)
		{
			return false;
		}
	}
	HitArray.Add(InActor);
	return true;
}

void UCGameplayAbility_Melee::ClearHitArray()
{
	HitArray.Empty();
}

void UCGameplayAbility_Melee::CollisionEnabled()
{
	CurrentSamples.Empty();
	PreviousSamples.Empty();
	ACMeleeWeapon* weapon = Cast<ACMeleeWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	if (!weapon)
	{
		return;
	}
	TArray<UCapsuleComponent*> capsules = weapon->CapsuleArray;
	CurrentSamples.SetNum(capsules.Num());
	PreviousSamples.SetNum(capsules.Num());
	GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &UCGameplayAbility_Melee::CheckCollision, GetWorld()->DeltaTimeSeconds, true);
}

void UCGameplayAbility_Melee::CollisionDisabled()
{
	GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
	CurrentSamples.Empty();
	PreviousSamples.Empty();
}

void UCGameplayAbility_Melee::CheckCollision()
{
	ACMeleeWeapon* weapon = Cast<ACMeleeWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	if (!weapon)
	{
		return;
	}
	TArray<UCapsuleComponent*> capsules = weapon->CapsuleArray;
	for (int j=0;j<capsules.Num();j++)
	{
		UCapsuleComponent* capsule = capsules[j];
		FVector upVector = capsule->GetUpVector();
		float sampleSize = capsule->GetScaledCapsuleHalfHeight() / SampleNumbers;
		FVector start = capsule->GetComponentLocation() + upVector * (sampleSize * (SampleNumbers / 2));

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
		TEnumAsByte<EObjectTypeQuery> Destructible = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Destructible);
		ObjectTypes.Add(Pawn);
		ObjectTypes.Add(Destructible);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetAvatarActorFromActorInfo());

		for (int i = 0; i < SampleNumbers; i++)
		{
			FVector temp = start - (i * (upVector * sampleSize));
			CurrentSamples[j].Push(temp);
		}
		if (PreviousSamples[j].Num() == 0)
		{
			PreviousSamples[j] = CurrentSamples[j];
		}
		for (int i = 0; i < SampleNumbers; i++)
		{
			TArray<FHitResult> OutHits;
			UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), CurrentSamples[j][i], PreviousSamples[j][i],
				ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, OutHits, true);

			for (FHitResult& OutHit : OutHits)
			{
				if (!HitArray.Contains(OutHit.GetActor()))
				{
					OnCollisionTraced(OutHit);
				}
			}
		}
		TArray<FHitResult> OutHits;
		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());
		CollisionQueryParams.AddIgnoredActor(weapon);
		GetWorld()->SweepMultiByObjectType(OutHits, capsule->GetComponentLocation(), capsule->GetComponentLocation(), capsule->GetComponentQuat(),
			CollisionObjectQueryParams,FCollisionShape::MakeCapsule(capsule->GetScaledCapsuleRadius(), capsule->GetScaledCapsuleHalfHeight()), CollisionQueryParams);
		//UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), capsule->GetComponentLocation(), capsule->GetScaledCapsuleHalfHeight(), capsule->GetScaledCapsuleRadius(), capsule->GetComponentRotation(),FLinearColor::Red, 0.1, 0.1);
		for (FHitResult& OutHit : OutHits)
		{
			if (!HitArray.Contains(OutHit.GetActor()))
			{
				OnCollisionTraced(OutHit);
			}
		}
		PreviousSamples[j] = CurrentSamples[j];
		CurrentSamples[j].Empty();
	}
}

UAnimMontage* UCGameplayAbility_Melee::GetBaseAttackMontage() const
{
	ACMeleeWeapon* weapon = Cast<ACMeleeWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	return weapon->GetWeaponDataAsset()->BaseAttackMontages[CurrentComboNum];
}

int UCGameplayAbility_Melee::GetMaxComboNum() const 
{
	ACMeleeWeapon* weapon = Cast<ACMeleeWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	return weapon->GetWeaponDataAsset()->BaseAttackMontages.Num()-1;
}

float UCGameplayAbility_Melee::GetAttackSpeed() const
{
	ACMeleeWeapon* weapon = Cast<ACMeleeWeapon>(OwnerCombatComponent->GetCurrentWeapon());
	const UCAttributeSet* AttributeSet= GetAbilitySystemComponentFromActorInfo()->GetSet<UCAttributeSet>();
	return weapon->GetWeaponDataAsset()->BaseAttackSpeed + AttributeSet->GetDexterity() * 0.1;
}


		
