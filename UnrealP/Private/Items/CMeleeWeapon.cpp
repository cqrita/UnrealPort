// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CMeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "LevelEditor.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"

ACMeleeWeapon::ACMeleeWeapon():
	Super()
{
	FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>("FieldSystemComponent");
	RadialVector = CreateDefaultSubobject<URadialVector>("RadialVector");
	RadialFalloff = CreateDefaultSubobject<URadialFalloff>("RadialFalloff");
}

void ACMeleeWeapon::OnConstruction(const FTransform& Transform)
{
	TInlineComponentArray<UCapsuleComponent*> capsules;
	GetComponents<UCapsuleComponent>(capsules, true);
	CapsuleArray.Append(capsules);
	if (!EquipmentSet)
	{
		return;
	}
	if (!Cast<ACWeapon>(EquipmentSet))
	{
		return;
	}
	capsules.Empty();
	EquipmentSet->GetComponents<UCapsuleComponent>(capsules, true);
	CapsuleArray.Append(capsules);
}

void ACMeleeWeapon::SetEquipmentSet(ACEquipment* Equipment)
{
	Super::SetEquipmentSet(Equipment);
	TInlineComponentArray<UCapsuleComponent*> capsules;
	EquipmentSet->GetComponents<UCapsuleComponent>(capsules, true);
	CapsuleArray.Append(capsules);
}
