// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/CUserWidget.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "CAttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALP_API UCAttributeWidget : public UCUserWidget
{
	GENERATED_BODY()
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner) override;
protected:
	void OnAttributeChanged(const FOnAttributeChangeData& ChangeData);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FGameplayAttribute, float> PrimaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayAttribute, float> SecondaryAttributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentAttributePoints = 0.0f;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAttributeMenu();
	UFUNCTION(BlueprintCallable)
	void OnPointSpent(FGameplayAttribute GameplayAttribute);
};
