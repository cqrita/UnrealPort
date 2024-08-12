// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

/**
 * 
 */
struct UNREALP_API FCGlobalTags : public FGameplayTagNativeAdder
{
	FGameplayTag DamageTag;
	FORCEINLINE static const FCGlobalTags& Get() { return GlobalTags; }
protected:
	//Called to register and assign the native tags
	virtual void AddTags() override;
private:
	//Private static object for the global tags. Use the Get() function to access externally.
	static FCGlobalTags GlobalTags;
};
