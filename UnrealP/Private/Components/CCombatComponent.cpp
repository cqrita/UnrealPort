// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CCombatComponent.h"
#include "Items/CWeapon.h"
#include "Interface/IItemInterface.h"
#include "Components/CItemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DataSets/CCombatDataAsset.h"
// Sets default values for this component's properties
UCCombatComponent::UCCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	bIsAttacking = false;
	bIsBlocking = false;
	// ...
}


// Called when the game starts
void UCCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	IIItemInterface* ItemInterface = Cast<IIItemInterface>(GetOwner());
	if (ItemInterface != nullptr)
	{
		ItemInterface->GetItemComponent()->SetNewWeapon.AddDynamic(this, &UCCombatComponent::EquipWeapon);
	}
	// ...
	bIsAttacking = false;
}

void UCCombatComponent::OnRegister()
{
	Super::OnRegister();
	AbilitySystemComponent = Cast<IAbilitySystemInterface>(GetOwner())->GetAbilitySystemComponent();
}


// Called every frame
void UCCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UCCombatComponent::EquipWeapon(ACWeapon* Weapon, TArray<TSubclassOf<UGameplayAbility>> GrantedAblities)
{
	CurrentWeapon = Weapon;
	if (Weapon != nullptr)
	{
		for (TSubclassOf<UGameplayAbility> GrantedAblity: GrantedAblities)
		{
			FGameplayAbilitySpec GrantedSpec= FGameplayAbilitySpec(GrantedAblity, 1);
			FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(GrantedSpec);
			SpecHandles.Add(SpecHandle);
		}
	}
	else
	{
		TArray<FGameplayAbilitySpecHandle> TempArray;
		for (TSubclassOf<UGameplayAbility> GrantedAblity : GrantedAblities)
		{
			for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandles)
			{
				bool isInstanced = true;
				if (AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle))
				{
					UClass* AbilityClass = UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilitySystemComponent, SpecHandle, isInstanced)->GetClass();
					if (GrantedAblity == AbilityClass)
					{
						AbilitySystemComponent->ClearAbility(SpecHandle);
						TempArray.Add(SpecHandle);
					}
				}
			}
		}
		for (FGameplayAbilitySpecHandle& TempHandle : TempArray)
		{
			SpecHandles.Remove(TempHandle);
		}
	}
	if (OnChangeWeapon.IsBound())
	{
		OnChangeWeapon.Broadcast(Weapon);
	}
}

ACWeapon* UCCombatComponent::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

void UCCombatComponent::SetAblitySystemComponent(UAbilitySystemComponent* Component)
{
	AbilitySystemComponent = Component;
}

void UCCombatComponent::AddCharacterStartupAbilities()
{
	if (CombatDataAsset)
	{
		for (TSubclassOf<UGameplayAbility> Ability : CombatDataAsset->StartupAbilities)
		{
			FGameplayAbilitySpec GrantedSpec = FGameplayAbilitySpec(Ability, 1);
			FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(GrantedSpec);
			SpecHandles.Add(SpecHandle);
		}
	}
}

void UCCombatComponent::ComboCall()
{
	OnComboCall.Broadcast();
}

void UCCombatComponent::BlockEnd()
{
	OnBlockEnd.Broadcast();
}

void UCCombatComponent::ProjectileFire()
{
	OnProjectileFire.Broadcast();
}
