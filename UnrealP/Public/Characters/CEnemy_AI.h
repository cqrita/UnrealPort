// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "Interface/ITargetInterface.h"
#include "CEnemy_AI.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API ACEnemy_AI : public ACEnemy, public IITargetInterface
{
	GENERATED_BODY()

	ACEnemy_AI();
public:
	virtual ACharacter* GetTarget() override;
};
