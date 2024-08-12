// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CParkourComponent.h"
#include "Interface/IParkourInterface.h"
#include "Interface/ICombatInterface.h"
#include "Components/CCombatComponent.h"
#include "Items/CWeapon.h"
#include "Items/CWeaponDataAsset.h"
#include "Animation/BlendSpace.h"
#include "Animation/BlendSpace1D.h"
#include "Interface/IStatusInterface.h"
#include "Components/CStatusComponent.h"
void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ACharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}
	MovementComponent = Cast<UCharacterMovementComponent>(Character->GetMovementComponent());
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(GetOwningActor());
	if (!!CombatInterface)
	{
		CombatComponent = CombatInterface->GetCombatComponent();
		if (!!CombatComponent)
		{
			CombatComponent->OnChangeWeapon.AddUniqueDynamic(this, &UCAnimInstance::OnWeaponChange);
		}
	}
	CurrentBlendSpace = BaseBlendSpace;
	if (Cast<UBlendSpace1D>(CurrentBlendSpace))
	{
		bIsOneDimension = true;
	}
	else
	{
		bIsOneDimension = false;
	}
	IIParkourInterface* ParkourInterface= Cast<IIParkourInterface>(GetOwningActor());
	if (!!ParkourInterface)
	{
		ParkourComponent = ParkourInterface->GetParkourComponent();
	}
	IIStatusInterface* StatusInterface = Cast<IIStatusInterface>(GetOwningActor());
	if (!!StatusInterface)
	{
		StatusComponent = StatusInterface->GetStatusComponent();
	}
}

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!Character)
	{
		return;
	}
	Velocity = Character->GetVelocity();
	Speed = Velocity.Size2D();
	Direction = UKismetAnimationLibrary::CalculateDirection(Character->GetVelocity(), Character->GetControlRotation());
	Pitch = Character->GetBaseAimRotation().Pitch;
	bIsCrouching = Character->bIsCrouched;
	if (!MovementComponent)
	{
		int i = 1;
		return;
	}
	if ((!MovementComponent->GetCurrentAcceleration().IsNearlyZero()) && (Speed > 0.1))
	{
		bShouldMove = true;
	}
	else
	{
		bShouldMove = false;
	}

	if (MovementComponent->IsFalling())
	{
		bIsFalling = true;
	}
	else
	{
		bIsFalling = false;
	}

	if (!!ParkourComponent)
	{
		if (ParkourComponent->GetParkourType() == EParkourType::EMax)
		{
			bIsParkour = false;
		}
		else
		{
			bIsParkour = true;
		}
	}
	if (!!StatusComponent)
	{
		bIsAlive = StatusComponent->GetIsAlive();
	}
}

void UCAnimInstance::OnWeaponChange(ACWeapon* Weapon)
{
	if (Weapon != nullptr)
	{
		if (!Weapon->GetWeaponDataAsset())
		{
			CurrentBlendSpace = BaseBlendSpace;
		}
		else
		{
			CurrentBlendSpace = Weapon->GetWeaponDataAsset()->WeaponBlendSpace;
		}
	}
	else
	{
		CurrentBlendSpace = BaseBlendSpace;
	}
	if (Cast<UBlendSpace1D>(CurrentBlendSpace))
	{
		bIsOneDimension = true;
	}
	else
	{
		bIsOneDimension = false;
	}
}
