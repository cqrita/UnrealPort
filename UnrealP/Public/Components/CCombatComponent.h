// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "CCombatComponent.generated.h"

class ACWeapon;
class ACCharacterBase;
class UCCombatDataAsset;
class UGameplayAbility;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComboCall);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBlockEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeWeapon, ACWeapon*, NewWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileFire);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<ACWeapon> CurrentWeapon;
public:
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(ACWeapon* Weapon, TArray<TSubclassOf<UGameplayAbility>> GrantedAblities);
	UFUNCTION(BlueprintCallable)
	ACWeapon* GetCurrentWeapon() const;
	UPROPERTY(BlueprintAssignable)
	FChangeWeapon OnChangeWeapon;
	UFUNCTION()
	void SetAblitySystemComponent(UAbilitySystemComponent* Component);
public:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> SpecHandles;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
public:
	void AddCharacterStartupAbilities();
public:
	UPROPERTY(BlueprintAssignable)
	FComboCall OnComboCall;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsAttacking=false;
public:
	UFUNCTION(BlueprintCallable)
	void ComboCall();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsAttacking(bool IsAttacking) { bIsAttacking = IsAttacking; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }


public:
	UPROPERTY(BlueprintAssignable)
	FBlockEnd OnBlockEnd;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBlocking=false;
public:
	UFUNCTION(BlueprintCallable)
	void BlockEnd();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsBlocking(bool IsBlocking) { bIsBlocking = IsBlocking; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsBlocking() const { return bIsBlocking; }

public:
	UPROPERTY(BlueprintAssignable)
	FProjectileFire OnProjectileFire;
	UFUNCTION(BlueprintCallable)
	void ProjectileFire();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCCombatDataAsset> CombatDataAsset;
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCCombatDataAsset* GetCombatDataAsset() const { return CombatDataAsset; }

};
