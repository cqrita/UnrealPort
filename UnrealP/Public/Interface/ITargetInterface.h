// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITargetInterface.generated.h"
class ACharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UITargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALP_API IITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual ACharacter* GetTarget() = 0;
};
