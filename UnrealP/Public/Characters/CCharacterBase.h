// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/ICombatInterface.h"
#include "Interface/IParkourInterface.h"
#include "Interface/IItemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/IStatusInterface.h"
#include "CCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UCCombatComponent;
class UCParkourComponent;
class UCItemComponent;
class UGameplayEffect;
class UCStatusComponent;
UCLASS()
class UNREALP_API ACCharacterBase : public ACharacter, public IAbilitySystemInterface,public IICombatInterface
	, public IGenericTeamAgentInterface, public IIParkourInterface, public IIItemInterface, public IIStatusInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
public:
	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalGeneration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultLevel;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterStartupAbilities();


public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 TeamId = 0;

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

public:
	virtual UCCombatComponent* GetCombatComponent() const override;
	virtual UCParkourComponent* GetParkourComponent() const override;
	virtual UCItemComponent* GetItemComponent() const override;
	virtual UCStatusComponent* GetStatusComponent() const override;
	virtual bool GetIsAlive() const override;
	virtual void SetIsAlive(bool IsAlive) override;
	virtual EHitReactDirection GetHitReactDirection(const FVector& ImpactPoint) const override;
	virtual UCStatusDataAsset* GetStatusDataAsset() const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCCombatComponent> CombatComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCParkourComponent> ParkourComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCItemComponent> ItemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCStatusComponent> StatusComponent;
};
