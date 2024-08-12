// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/COverlayWidget.h"
#include "AbilitySystem/CAttributeSet.h"
#include "Interface/IItemInterface.h"
#include "Components/CItemComponent.h"
void UCOverlayWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetHealthAttribute()).AddUObject(this, &UCOverlayWidget::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UCOverlayWidget::OnMaxHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetManaAttribute()).AddUObject(this, &UCOverlayWidget::OnManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxManaAttribute()).AddUObject(this, &UCOverlayWidget::OnMaxManaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetStaminaAttribute()).AddUObject(this, &UCOverlayWidget::OnStaminaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UCAttributeSet::GetMaxStaminaAttribute()).AddUObject(this, &UCOverlayWidget::OnMaxStaminaChanged);

		const UCAttributeSet* CurrentCharacterAttributeSet = AbilitySystemComponent->GetSet<UCAttributeSet>();
		if (CurrentCharacterAttributeSet)
		{
			CurrentHealth = CurrentCharacterAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentCharacterAttributeSet->GetMaxHealth();
			CurrentMana = CurrentCharacterAttributeSet->GetMana();
			CurrentMaxMana = CurrentCharacterAttributeSet->GetMaxMana();
			CurrentStamina = CurrentCharacterAttributeSet->GetStamina();
			CurrentMaxStamina = CurrentCharacterAttributeSet->GetMaxStamina();

			UpdateHPBar();
			UpdateMPBar();
			UpdateStaminaBar();
		}
	}
}

void UCOverlayWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UCOverlayWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UCOverlayWidget::OnManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMana = ChangeData.NewValue;
	UpdateMPBar();
}

void UCOverlayWidget::OnMaxManaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxMana = ChangeData.NewValue;
	UpdateMPBar();
}

void UCOverlayWidget::OnStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}

void UCOverlayWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxStamina = ChangeData.NewValue;
	UpdateStaminaBar();
}

void UCOverlayWidget::SetItemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ItemComponent = Cast<IIItemInterface>(InOwner)->GetItemComponent();
		ItemComponent->SetNewItem.AddUniqueDynamic(this, &UCOverlayWidget::OnItemAdded);
	}
}
