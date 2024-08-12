// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CItemComponent.h"
#include "Items/CEquipment.h"
#include "Items/CWeapon.h"
#include "Characters/CCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Items/CWeaponDataAsset.h"

// Sets default values for this component's properties
UCItemComponent::UCItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UCItemComponent::BeginPlay()
{
	Super::BeginPlay();
	bOnEquipState = false;

	// ...
	
}

void UCItemComponent::OnRegister()
{
	Super::OnRegister();
	OwnerCharacter = Cast<ACCharacterBase>(GetOwner());
}


// Called every frame
void UCItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCItemComponent::EquipEquipment(ACEquipment* Equipment)
{
	bOnEquipState = true;
	if (TempEquipment&&MainEquipment==nullptr)
	{
		BeginEquipParams(Equipment);
		EndEquipParams(Equipment);
		return;
	}
	TempEquipment = Equipment;
	ACWeapon* newWeapon = Cast<ACWeapon>(Equipment);
	if (newWeapon)
	{
		if (newWeapon->GetWeaponDataAsset()->EquipAbility)
		{
			if (!OwnerCharacter->GetAbilitySystemComponent())
			{
				BeginEquip();
				EndEquip();
			}
			else if (!(OwnerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(newWeapon->GetWeaponDataAsset()->EquipAbility)))
			{
				BeginEquip();
				EndEquip();
			}
			else
			{
				bNotifyFired = false;
			}
		}
		else
		{
			BeginEquip();
			EndEquip();
		}
	}
	else
	{
		BeginEquip();
		EndEquip();
	}
}

void UCItemComponent::BeginEquip()
{
	TempEquipment->EquipEquipment(OwnerCharacter->GetMesh());
	ACWeapon* newWeapon = Cast<ACWeapon>(TempEquipment);
	TArray<TSubclassOf<UGameplayAbility>> Ablities;
	if (newWeapon)
	{
		ACEquipment* EquipmentSet = newWeapon->EquipmentSet;
		if (EquipmentSet)
		{
			EquipEquipment(EquipmentSet);
		}
		else if (newWeapon->GetWeaponDataAsset()->EquipmentSetClass)
		{
			newWeapon->SetEquipmentSet(Cast<ACEquipment>(OwnerCharacter->GetWorld()->SpawnActor(newWeapon->GetWeaponDataAsset()->EquipmentSetClass)));
			EquipEquipment(newWeapon->EquipmentSet);
		}
		if (OwnerCharacter->GetAbilitySystemComponent())
		{
			if (newWeapon->GetWeaponDataAsset())
			{
				for (TSubclassOf<UGameplayAbility> GrantedAbliity : newWeapon->GetWeaponDataAsset()->GrantedAblities)
				{
					Ablities.Add(GrantedAbliity);
				}
			}
		}
		SetNewWeapon.Broadcast(newWeapon, Ablities);
	}
	bNotifyFired = true;
}

void UCItemComponent::EndEquip()
{
	Equipments.Add(TempEquipment);
	SetNewEquipment.Broadcast(TempEquipment);
	MainEquipment = TempEquipment;
	TempEquipment = nullptr;
	bOnEquipState = false;
}

void UCItemComponent::BeginEquipParams(ACEquipment* Equipment)
{
	Equipment->EquipEquipment(OwnerCharacter->GetMesh());
}

void UCItemComponent::EndEquipParams(ACEquipment* Equipment)
{
	Equipments.Add(Equipment);
	SetNewEquipment.Broadcast(Equipment);
}

void UCItemComponent::UnequipEquipment(ACEquipment* Equipment)
{
	bOnEquipState = true;
	if (TempEquipment && MainEquipment != nullptr)
	{
		BeginUnequipParams(Equipment);
		EndUnequipParams(Equipment);
		return;
	}
	TempEquipment = Equipment;
	ACWeapon* newWeapon = Cast<ACWeapon>(Equipment);
	if (newWeapon)
	{
		if (newWeapon->GetWeaponDataAsset()->UnequipAbility)
		{
			if (!(OwnerCharacter->GetAbilitySystemComponent()->TryActivateAbilityByClass(newWeapon->GetWeaponDataAsset()->UnequipAbility)))
			{
				BeginUnequip();
				EndUnequip();
			}
			else
			{
				bNotifyFired = false;
			}
		}
		else
		{
			BeginUnequip();
			EndUnequip();
		}
	}
	else
	{
		BeginUnequip();
		EndUnequip();
	}
}

void UCItemComponent::BeginUnequip()
{
	TempEquipment->UnequipEquipment(OwnerCharacter->GetMesh());
	ACWeapon* newWeapon = Cast<ACWeapon>(TempEquipment);
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	if (newWeapon)
	{
		ACEquipment* EquipmentSet = newWeapon->EquipmentSet;
		if (EquipmentSet)
		{
			UnequipEquipment(EquipmentSet);
		}
		if (OwnerCharacter->GetAbilitySystemComponent())
		{
			if (newWeapon->GetWeaponDataAsset())
			{
				for (TSubclassOf<UGameplayAbility> GrantedAbiliity : newWeapon->GetWeaponDataAsset()->GrantedAblities)
				{
					Abilities.Add(GrantedAbiliity);
				}
			}
		}
		if(MainEquipment==TempEquipment)
		SetNewWeapon.Broadcast(nullptr, Abilities);
	}
	bNotifyFired = true;
}

void UCItemComponent::EndUnequip()
{
	Equipments.Remove(TempEquipment);
	SetNewEquipment.Broadcast(nullptr);
	MainEquipment = nullptr;
	TempEquipment = nullptr;
	bOnEquipState = false;
}

void UCItemComponent::BeginUnequipParams(ACEquipment* Equipment)
{
	Equipment->UnequipEquipment(OwnerCharacter->GetMesh());
}

void UCItemComponent::EndUnequipParams(ACEquipment* Equipment)
{
	Equipments.Remove(Equipment);
	SetNewEquipment.Broadcast(nullptr);
}

void UCItemComponent::AddItem(ACItem* Item)
{
	Items.Add(Item);
	SetNewItem.Broadcast();
}

void UCItemComponent::RemoveItem(ACItem* Item)
{
	Items.Remove(Item);
	SetNewItem.Broadcast();
}

TArray<ACItem*> UCItemComponent::GetItems() const
{
	return Items;
}

