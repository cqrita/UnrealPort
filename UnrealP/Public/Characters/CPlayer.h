// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacterBase.h"
#include "Interface/ITargetInterface.h"
#include "CPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCTargetComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCInputDataAsset;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class UNREALP_API ACPlayer : public ACCharacterBase, public IITargetInterface
{
	GENERATED_BODY()


public:
	ACPlayer();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCTargetComponent> TargetComponent;


public:
	virtual ACharacter* GetTarget() override;

//Temp
private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
protected:
	// To add mapping context
	virtual void BeginPlay();
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
protected:
	void SetupActionInput();
	void TriggerAction(const FInputActionValue& Value, TSubclassOf<UGameplayAbility> GameplayAbility);
	void ReleaseAction(const FInputActionValue& Value, TSubclassOf<UGameplayAbility> GameplayAbility);

	void InputEquipment(const FInputActionValue& Value, int EquipmentSlot);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCInputDataAsset> InputDataAsset;
};
