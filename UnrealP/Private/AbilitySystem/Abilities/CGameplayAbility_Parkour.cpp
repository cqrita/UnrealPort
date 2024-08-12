// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CGameplayAbility_Parkour.h"
#include "GameFramework/Character.h"
#include "Components/CParkourComponent.h"
#include "Interface/IParkourInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
UCGameplayAbility_Parkour::UCGameplayAbility_Parkour()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
void UCGameplayAbility_Parkour::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	IIParkourInterface* ParkourInterface = Cast<IIParkourInterface>(ActorInfo->AvatarActor);
	OwnerParkourComponent = ParkourInterface->GetParkourComponent();
}
EParkourType UCGameplayAbility_Parkour::DoParkour(bool bLanded)
{
	if (OwnerParkourComponent->GetParkourType() != EParkourType::EMax)
	{
		return EParkourType::EMax;
	}
	if (bLanded && OwnerParkourComponent->Check_FallMode())
	{
		DoParkour_Fall();

		return EParkourType::EFall;
	}
	OwnerParkourComponent->InitializeParkour();
	OwnerParkourComponent->CheckTrace_Center();

	if (!!OwnerParkourComponent->GetHitObstacle())
	{
		OwnerParkourComponent->CheckTrace_Ceil();
		OwnerParkourComponent->CheckTrace_Floor();
	}
	if (OwnerParkourComponent->Check_Obstacle() == false)
	{
		return EParkourType::EMax;
	}
	if (OwnerParkourComponent->Check_ClimbMode())
	{
		DoParkour_Climb();

		return EParkourType::EClimb;
	}
	if (OwnerParkourComponent->Check_ObstacleMode())
	{
		DoParkour_Obstacle();

		return EParkourType::ENormal;
	}
	return EParkourType::EMax;
}

void UCGameplayAbility_Parkour::End_DoParkour()
{
	if (OwnerParkourComponent->GetParkourType() == EParkourType::EClimb)
	{
		End_DoParkour_Climb();
	}
	else if (OwnerParkourComponent->GetParkourType() == EParkourType::ENormal)
	{
		End_DoParkour_Obstacle();
	}

	OwnerParkourComponent->SetParkourType(EParkourType::EMax);
}

void UCGameplayAbility_Parkour::DoParkour_Climb()
{
	OwnerParkourComponent->SetParkourType(EParkourType::EClimb);

	//OwnerCharacter->SetActorLocation(OwnerParkourComponent->HitResults[(int32)EParkourArrowType::ECenter].ImpactPoint);
	//OwnerCharacter->SetActorRotation(FRotator(0, OwnerParkourComponent->ToFrontYaw, 0));
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCGameplayAbility_Parkour::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

FTransform UCGameplayAbility_Parkour::DoParkour_Climb_Transform()
{
	OwnerParkourComponent->CheckTrace_Center();
	OwnerParkourComponent->CheckTrace_Ceil();
	FTransform ActorTransform;
	FRotator ActorRotator = OwnerCharacter->GetActorRotation();
	ActorRotator.Yaw = OwnerParkourComponent->ToFrontYaw;
	FVector ActorLocation = OwnerParkourComponent->HitResults[(int32)EParkourArrowType::ECenter].ImpactPoint;
	ActorTransform.SetRotation(ActorRotator.Quaternion());
	ActorTransform.SetLocation(ActorLocation);
	return ActorTransform;
}

bool UCGameplayAbility_Parkour::End_DoParkour_Climb_Check() const
{
	return !OwnerParkourComponent->HitResults[(int32)EParkourArrowType::ECeil].bBlockingHit;
}

void UCGameplayAbility_Parkour::DoParkour_Fall()
{
	OwnerParkourComponent->SetParkourType(EParkourType::EFall);
}

void UCGameplayAbility_Parkour::DoParkour_Slide()
{
	OwnerParkourComponent->SetParkourType(EParkourType::ESlide);
	OwnerCharacter->Crouch();
}

void UCGameplayAbility_Parkour::End_DoParkour_Slide()
{
	OwnerCharacter->UnCrouch();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UCGameplayAbility_Parkour::DoParkour_Obstacle()
{
	OwnerParkourComponent->SetParkourType(EParkourType::ENormal);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	TempHalfHeight=OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	OwnerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(TempHalfHeight/2);
}

void UCGameplayAbility_Parkour::End_DoParkour_Obstacle()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//OwnerCharacter->UnCrouch();
	OwnerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(TempHalfHeight);
	//OwnerCharacter->GetMesh()->SetSimulatePhysics(false);
}

FTransform UCGameplayAbility_Parkour::DoParkour_Obstacle_Transform()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	FTransform ActorTransform;
	FRotator ActorRotator = OwnerCharacter->GetActorRotation();
	ActorRotator.Yaw = OwnerParkourComponent->ToFrontYaw;
	FVector ActorLocation = OwnerParkourComponent->HitResults[(int32)EParkourArrowType::ECenter].ImpactPoint;
	ActorLocation = ActorLocation + OwnerCharacter->GetActorForwardVector() * 30;
	ActorLocation.Z = OwnerParkourComponent->HitObstacle->GetActorLocation().Z + OwnerParkourComponent->HitObstacleExtent.Z- OwnerCharacter->GetSimpleCollisionHalfHeight()+10;
	ActorTransform.SetRotation(ActorRotator.Quaternion());
	ActorTransform.SetLocation(ActorLocation);
	return ActorTransform;
}
