// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CHealthWidget.h"
#include "AbilitySystem/CAttributeSet.h"
void UCHealthWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetHealthAttribute()).AddUObject(this, &UCHealthWidget::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UCHealthWidget::OnMaxHealthChanged);

		const UCAttributeSet* CurrentCharacterAttributeSet = AbilitySystemComponent->GetSet<UCAttributeSet>();
		if (CurrentCharacterAttributeSet)
		{
			CurrentHealth = CurrentCharacterAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentCharacterAttributeSet->GetMaxHealth();
			UpdateHPBar();
		}
	}
}

void UCHealthWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UCHealthWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHPBar();
}
