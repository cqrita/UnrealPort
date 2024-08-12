// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CBehaviorComponent.h"
#
// Sets default values for this component's properties
UCBehaviorComponent::UCBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EBehaviorType UCBehaviorComponent::GetBehavior()
{
	return Behavior;
}

void UCBehaviorComponent::SetBehavior(EBehaviorType InBehavior)
{
	EBehaviorType type = GetBehavior();
	Behavior = InBehavior;
	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InBehavior);
}

