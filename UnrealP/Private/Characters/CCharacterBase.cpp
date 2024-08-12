// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CCharacterBase.h"
#include "Components/CCombatComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CItemComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "AIController.h"

// Sets default values
ACCharacterBase::ACCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CombatComponent = CreateDefaultSubobject<UCCombatComponent>("CombatComponent");
	ParkourComponent = CreateDefaultSubobject<UCParkourComponent>("ParkourComponent");
	ItemComponent = CreateDefaultSubobject<UCItemComponent>("ItemComponent");
	StatusComponent = CreateDefaultSubobject<UCStatusComponent>("StatusComponent");
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	if (GetMovementComponent())
	{
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}

// Called when the game starts or when spawned
void ACCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCCombatComponent* ACCharacterBase::GetCombatComponent() const
{
	return CombatComponent;
}

UCParkourComponent* ACCharacterBase::GetParkourComponent() const
{
	return ParkourComponent;
}

UCItemComponent* ACCharacterBase::GetItemComponent() const
{
	return ItemComponent;
}

UCStatusComponent* ACCharacterBase::GetStatusComponent() const
{
	return StatusComponent;
}

bool ACCharacterBase::GetIsAlive() const
{
	return StatusComponent->GetIsAlive();
}

void ACCharacterBase::SetIsAlive(bool IsAlive)
{
	StatusComponent->SetIsAlive(IsAlive);
}

EHitReactDirection ACCharacterBase::GetHitReactDirection(const FVector& ImpactPoint) const
{
	return StatusComponent->GetHitReactDirection(ImpactPoint);
}

UCStatusDataAsset* ACCharacterBase::GetStatusDataAsset() const
{
	return StatusComponent->GetStatusDataAsset();
}

void ACCharacterBase::InitAbilityActorInfo()
{
}

void ACCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalGeneration, 1.f);
	ApplyEffectToSelf(DefaultLevel, 1.f);
}

void ACCharacterBase::AddCharacterStartupAbilities()
{
	if (!HasAuthority()) return;
	CombatComponent->AddCharacterStartupAbilities();
}

FGenericTeamId ACCharacterBase::GetGenericTeamId() const
{
	return FGenericTeamId(TeamId);
}

void ACCharacterBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = TeamID.GetId();
	AAIController* ThisController= Cast<AAIController>(GetController());
	if (!!ThisController)
	{
		ThisController->SetGenericTeamId(TeamID);
	}
	
}

