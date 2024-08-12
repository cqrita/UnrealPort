// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Perception/AISense_Damage.h"
#include "Interface/ICombatInterface.h"
#include "Components/CCombatComponent.h"
#include "Interface/IStatusInterface.h"
#include "Components/CStatusComponent.h"
#include "DataSets/CStatusDataAsset.h"
#include "Components/CapsuleComponent.h"
void UCAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCAttributeSet, AttributePoints, COND_None, REPNOTIFY_Always);
}

void UCAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	AActor* TargetActor = nullptr;
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	UAbilitySystemComponent* Target = &Data.Target;
	ACharacter* TargetCharacter = nullptr;
	IIStatusInterface* TargetStatus = nullptr;
	APlayerController* TargetPlayerController = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ACharacter>(TargetActor);
		TargetStatus = Cast<IIStatusInterface>(TargetActor);
		TargetPlayerController = Data.Target.AbilityActorInfo->PlayerController.Get();
	}
	AActor* SourceActor = nullptr;

	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
	}

	IICombatInterface* TargetCombatInterface = Cast<IICombatInterface>(TargetActor);
	UCCombatComponent* TargetCombatComponent = TargetCombatInterface->GetCombatComponent();
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		bool WasAlive = true;

		if (TargetStatus)
		{
			WasAlive = TargetStatus->GetIsAlive();
		}
		const float NewHealth = GetHealth() - LocalDamageDone;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		if (NewHealth > 0.0f)
		{
			if (!TargetCombatComponent->GetIsBlocking())
			{
				if (HitResult.bBlockingHit)
				{
					EHitReactDirection HitDirection = TargetStatus->GetHitReactDirection(Data.EffectSpec.GetContext().GetHitResult()->Location);
					switch (HitDirection)
					{
					case EHitReactDirection::ELeft:
						TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnHitLeftMontage);
						break;
					case EHitReactDirection::EFront:
						TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnHitFrontMontage);
						break;
					case EHitReactDirection::ERight:
						TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnHitRightMontage);
						break;
					case EHitReactDirection::EBack:
						TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnHitBackMontage);
						break;
					}
				}
				else
				{
					TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnHitFrontMontage);
				}
			}
		}
		else
		{
			TargetStatus->SetIsAlive(false);
			if (WasAlive)
			{
				if (TargetPlayerController == nullptr)
				{
					TargetCharacter->UnPossessed();
				}
				TargetCharacter->StopAnimMontage(TargetCharacter->GetCurrentMontage());
				TargetCharacter->PlayAnimMontage(TargetStatus->GetStatusDataAsset()->OnDeathMontage);
				TargetCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if (TargetStatus->GetStatusDataAsset()->OnDeathXP)
				{
					UGameplayEffect* XPEffect = TargetStatus->GetStatusDataAsset()->OnDeathXP->GetDefaultObject<UGameplayEffect>();
					Target->ApplyGameplayEffectToTarget(XPEffect, Source);
				}
			}
		}
		UAISense_Damage::ReportDamageEvent(GetWorld(), TargetActor, SourceActor, LocalDamageDone, Context.GetOrigin(), HitResult.Location);
	}
	if (Data.EvaluatedData.Attribute == GetXPAttribute())
	{
		float LocalXP = GetXP();
		float LocalLevel = GetLevel();
		while (LocalXP >= LocalLevel && LocalXP>0)
		{
			LocalXP = LocalXP - LocalLevel;
			LocalLevel = LocalLevel - 1;
			SetLevel(GetLevel() + 1);
			SetAttributePoints(GetAttributePoints() + 1);
		}
		SetXP(LocalXP);
	}
}

void UCAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Health, OldHealth);
}

void UCAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Mana, OldMana);
}

void UCAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Strength, OldStrength);
}

void UCAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Intelligence, OldIntelligence);
}

void UCAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Dexterity, OldDexterity);
}

void UCAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, MaxHealth, OldMaxHealth);
}

void UCAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, MaxMana, OldMaxMana);
}

void UCAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Armor, OldArmor);
}

void UCAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Stamina, OldStamina);
}

void UCAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, MaxStamina, OldMaxStamina);
}

void UCAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, Level, OldLevel);
}

void UCAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, XP, OldXP);
}

void UCAttributeSet::OnRep_AttributePoints(const FGameplayAttributeData& OldAttributePoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCAttributeSet, AttributePoints, OldAttributePoints);
}
