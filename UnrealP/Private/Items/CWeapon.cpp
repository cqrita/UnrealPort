// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CWeapon.h"
#include "Items/CWeaponDataAsset.h"
#include "Items/CEquipmentDataAsset.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

ACWeapon::ACWeapon():
	Super()
{
	
}
void ACWeapon::SetEquipmentSet(ACEquipment* Equipment)
{
	EquipmentSet = Equipment;
}
UCWeaponDataAsset* ACWeapon::GetWeaponDataAsset() const
{
	return Cast<UCWeaponDataAsset>(EquipmentDataAsset);
}
