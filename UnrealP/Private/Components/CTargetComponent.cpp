// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CTargetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCTargetComponent::UCTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<ACharacter>(GetOwner());
	// ...
}


// Called when the game starts
void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TempTarget = nullptr;
	Closest = Distance * 10;
	FVector Start = Owner->GetActorLocation()+(UKismetMathLibrary::GetForwardVector(Owner->GetBaseAimRotation())*Forward);
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(Owner->GetBaseAimRotation()) * Distance);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(Pawn);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);
	TArray<FHitResult> OutHits;

	bool bFlag = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, Radius
		, ObjectTypes, false, IgnoreActors, DrawDebugTrace, OutHits, true);
	for (FHitResult& OutHit : OutHits)
	{
		ACharacter* Character = Cast<ACharacter>(OutHit.GetActor());
		if(!Character)
		{ 
			return;
		}
		float TempDistance = FVector::Distance(Owner->GetActorLocation(),Character->GetActorLocation());
		if (TempDistance < Closest)
		{
			Closest = TempDistance;
			TempTarget = Character;
		}
	}
	if (TempTarget)
	{
		Target = TempTarget;
	}
	// ...
}

