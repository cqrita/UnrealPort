// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

void UCUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}

}

UAbilitySystemComponent* UCUserWidget::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
