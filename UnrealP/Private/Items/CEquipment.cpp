// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CEquipment.h"
ACEquipment::ACEquipment()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}
void ACEquipment::EquipEquipment(USkeletalMeshComponent* OwnerMesh)
{
	StaticMeshComponent->SetVisibility(true, true);
	AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
}

void ACEquipment::UnequipEquipment(USkeletalMeshComponent* OwnerMesh)
{
	if (SheathSocketName.IsNone())
	{
		StaticMeshComponent->SetVisibility(false, true);
	}
	else
	{
		AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SheathSocketName);
	}
}
