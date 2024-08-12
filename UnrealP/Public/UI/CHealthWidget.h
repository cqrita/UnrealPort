// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UI/CUserWidget.h"
#include "CHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API UCHealthWidget : public UCUserWidget
{
	GENERATED_BODY()
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHPBar();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float CurrentHealth = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentMaxHealth = 0.1;

};
