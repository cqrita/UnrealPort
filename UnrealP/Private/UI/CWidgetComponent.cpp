// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CWidgetComponent.h"
#include "UI/CUserWidget.h"
void UCWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UCUserWidget* UserWidget = Cast<UCUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetAbilitySystemComponent(GetOwner());
	}

}
