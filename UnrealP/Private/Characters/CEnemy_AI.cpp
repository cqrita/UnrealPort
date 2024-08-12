// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy_AI.h"
#include "AI/CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
ACEnemy_AI::ACEnemy_AI():
	Super()
{
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	UCharacterMovementComponent* const MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}
	AIControllerClass = TSubclassOf<ACAIController>();
}
ACharacter* ACEnemy_AI::GetTarget()
{
	ACAIController* AIController= Cast<ACAIController>(GetController());
	if (!AIController)
	{
		return nullptr;
	}
	ACharacter* rv= Cast<ACharacter>(AIController->Target);
	return rv;
}