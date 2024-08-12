// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CUserWidget.h"
#include "GameplayEffectTypes.h"
#include "COverlayWidget.generated.h"
class UCItemComponent;
/**
 * 
 */
UCLASS()
class UNREALP_API UCOverlayWidget : public UCUserWidget
{
	GENERATED_BODY()
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
private:
	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	void OnManaChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxManaChanged(const FOnAttributeChangeData& ChangeData);
	void OnStaminaChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData);
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHPBar();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMPBar();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStaminaBar();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHealth = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMaxHealth = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMana = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMaxMana = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentStamina = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMaxStamina = 0.1;
protected:
	UFUNCTION(BlueprintCallable)
	void SetItemComponent(AActor* InOwner);
	UFUNCTION(BlueprintImplementableEvent)
	void OnItemAdded();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCItemComponent> ItemComponent;
};
