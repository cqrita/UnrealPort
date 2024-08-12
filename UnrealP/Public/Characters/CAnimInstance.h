// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;
class UCParkourComponent;
class UCCombatComponent;
class UCStatusComponent;
class UBlendSpace;
class ACWeapon;
/**
 * 
 */
UCLASS()
class UNREALP_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouching;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCParkourComponent> ParkourComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsParkour;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCCombatComponent> CombatComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBlendSpace> BaseBlendSpace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> CurrentBlendSpace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsOneDimension;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCStatusComponent> StatusComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void OnWeaponChange(ACWeapon* Weapon);
};
