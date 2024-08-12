// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
// Sets default values for this component's properties
UCStatusComponent::UCStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsAlive = true;
	// ...
}


// Called when the game starts
void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCStatusComponent::SetIsAlive(bool IsAlive)
{
	bIsAlive = IsAlive;
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (!owner)
	{
		return;
	}	
	if (IsAlive)
	{
		owner->EnableInput(nullptr);
	}
	else
	{
		owner->DisableInput(nullptr);
	}
}

EHitReactDirection UCStatusComponent::GetHitReactDirection(const FVector& ImpactPoint) const
{
	AActor* OwnerActor = GetOwner();
	const FVector& ActorLocation = OwnerActor->GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, OwnerActor->GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, OwnerActor->GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return EHitReactDirection::EFront;
		}
		else
		{
			return EHitReactDirection::EBack;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return EHitReactDirection::ERight;
		}
		else
		{
			return EHitReactDirection::ELeft;
		}
	}

	return EHitReactDirection::EFront;
}


