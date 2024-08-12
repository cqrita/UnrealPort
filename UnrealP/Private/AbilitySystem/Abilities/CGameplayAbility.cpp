// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility.h"
#include "Interface/ICombatInterface.h"
#include "Components/CCombatComponent.h"
#include "Items/CWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/ITargetInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/Actors/CProjectile.h"

UCGameplayAbility::UCGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	OwnerCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);
}
FTransform UCGameplayAbility::GetWarpTransform(float Distance, float WarpDistance, bool& bOutOfDistance, bool& bOnlyRotate)
{
	bOutOfDistance = true;
	bOnlyRotate = false;
	FTransform RV;
	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	IITargetInterface* TargetInterface = Cast<IITargetInterface>(OwnerActor);
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FRotator OwnerRotation = OwnerActor->GetActorRotation();
	RV.SetRotation(OwnerRotation.Quaternion());
	RV.SetLocation(OwnerLocation);
	ACharacter* TargetCharacter = TargetInterface->GetTarget();
	if (!TargetCharacter)
	{
		return RV;
	}
	FVector TargetLocation = TargetCharacter->GetActorLocation();
	if (FVector::Dist(OwnerLocation, TargetLocation) > Distance)
	{
		return RV;
	}
	bOutOfDistance = false;
	FRotator ReturnRotation= UKismetMathLibrary::FindLookAtRotation(OwnerLocation, TargetLocation);
	ReturnRotation.Pitch = 0;
	RV.SetRotation(ReturnRotation.Quaternion());
	if (FVector::Dist(OwnerLocation, TargetLocation) < WarpDistance)
	{
		bOnlyRotate = true;
		return RV;
	}
	FVector ReturnLocation = TargetLocation - UKismetMathLibrary::GetForwardVector(ReturnRotation) * WarpDistance;
	RV.SetLocation(ReturnLocation);
	return RV;
}

ACharacter* UCGameplayAbility::GetEnemyTarget()
{
	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	IITargetInterface* TargetInterface = Cast<IITargetInterface>(OwnerActor);
	ACharacter* TargetCharacter = TargetInterface->GetTarget();
	return TargetCharacter;
}

