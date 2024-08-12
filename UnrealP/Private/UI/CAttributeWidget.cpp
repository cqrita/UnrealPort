// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAttributeWidget.h"
#include "AbilitySystem/CAttributeSet.h"

void UCAttributeWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (AbilitySystemComponent)
	{
		const UCAttributeSet* CurrentCharacterAttributeSet = AbilitySystemComponent->GetSet<UCAttributeSet>();
		if (CurrentCharacterAttributeSet)
		{
			PrimaryAttributes.Add(UCAttributeSet::GetStrengthAttribute(), CurrentCharacterAttributeSet->GetStrength());
			PrimaryAttributes.Add(UCAttributeSet::GetIntelligenceAttribute(), CurrentCharacterAttributeSet->GetIntelligence());
			PrimaryAttributes.Add(UCAttributeSet::GetDexterityAttribute(), CurrentCharacterAttributeSet->GetDexterity());

			SecondaryAttributes.Add(UCAttributeSet::GetMaxHealthAttribute(), CurrentCharacterAttributeSet->GetMaxHealth());
			SecondaryAttributes.Add(UCAttributeSet::GetMaxManaAttribute(), CurrentCharacterAttributeSet->GetMaxMana());
			SecondaryAttributes.Add(UCAttributeSet::GetMaxStaminaAttribute(), CurrentCharacterAttributeSet->GetMaxStamina());
			SecondaryAttributes.Add(UCAttributeSet::GetArmorAttribute(), CurrentCharacterAttributeSet->GetArmor());

			CurrentAttributePoints = CurrentCharacterAttributeSet->GetAttributePoints();
		}

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetStrengthAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetIntelligenceAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetDexterityAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetArmorAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetAttributePointsAttribute()).AddUObject(this, &UCAttributeWidget::OnAttributeChanged);

		UpdateAttributeMenu();
	}
}

void UCAttributeWidget::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	for (TPair<FGameplayAttribute, float>& AttributePairs : PrimaryAttributes)
	{
		if (ChangeData.Attribute == AttributePairs.Key)
		{
			AttributePairs.Value= ChangeData.NewValue;
		}
	}
	for (TPair<FGameplayAttribute, float>& AttributePairs : SecondaryAttributes)
	{
		if (ChangeData.Attribute == AttributePairs.Key)
		{
			AttributePairs.Value = ChangeData.NewValue;
		}
	}
	if (ChangeData.Attribute == UCAttributeSet::GetAttributePointsAttribute())
	{
		CurrentAttributePoints = ChangeData.NewValue;
	}
	UpdateAttributeMenu();
}

void UCAttributeWidget::OnPointSpent(FGameplayAttribute GameplayAttribute)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ApplyModToAttribute(GameplayAttribute, EGameplayModOp::Additive, 1);
		AbilitySystemComponent->ApplyModToAttribute(UCAttributeSet::GetAttributePointsAttribute(), EGameplayModOp::Additive, - 1);
	}
	UpdateAttributeMenu();
}

