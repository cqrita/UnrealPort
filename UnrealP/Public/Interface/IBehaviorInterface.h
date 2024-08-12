// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/CBehaviorComponent.h"
#include "IBehaviorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UIBehaviorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALP_API IIBehaviorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual EBehaviorType GetBehavior() = 0;
	UFUNCTION(BlueprintCallable)
	virtual void SetBehavior(EBehaviorType InBehavior) = 0;

};
