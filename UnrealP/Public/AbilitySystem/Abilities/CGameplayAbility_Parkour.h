// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CGameplayAbility.h"
#include "CGameplayAbility_Parkour.generated.h"

class UCParkourComponent;
/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility_Parkour : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	UCGameplayAbility_Parkour();
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	UPROPERTY()
	TObjectPtr<UCParkourComponent> OwnerParkourComponent;
public:
	UFUNCTION(BlueprintCallable)
	EParkourType DoParkour(bool bLanded = false);
	UFUNCTION(BlueprintCallable)
	void End_DoParkour();
private:
	void DoParkour_Climb();
	void End_DoParkour_Climb();
public:
	UFUNCTION(BlueprintCallable)
	FTransform DoParkour_Climb_Transform();
	UFUNCTION(BlueprintCallable)
	bool End_DoParkour_Climb_Check() const;
public:
	UFUNCTION(BlueprintCallable)
	void DoParkour_Fall();
public:
	UFUNCTION(BlueprintCallable)
	void DoParkour_Slide();
	UFUNCTION(BlueprintCallable)
	void End_DoParkour_Slide();
private:
	void DoParkour_Obstacle();
	void End_DoParkour_Obstacle();
public:
	UFUNCTION(BlueprintCallable)
	FTransform DoParkour_Obstacle_Transform();
private:
	UPROPERTY()
	float TempHalfHeight;
};
