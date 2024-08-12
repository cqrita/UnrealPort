// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CEnemy.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "AbilitySystem/CAttributeSet.h"
#include "UI/CHealthWidget.h"
#include "UI/CWidgetComponent.h"
ACEnemy::ACEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCAttributeSet>("AttributeSet");

	HpBar = CreateDefaultSubobject<UCWidgetComponent>(TEXT("WidgetComponent"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HpBar->SetWidgetClass(WidgetClass);
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}

void ACEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
		AddCharacterStartupAbilities();
	}
}
