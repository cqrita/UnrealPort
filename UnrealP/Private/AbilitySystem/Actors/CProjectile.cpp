// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Actors/CProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACProjectile::ACProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_WorldDynamic);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

// Called when the game starts or when spawned
void ACProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile::OnSphereOverlap);
}

void ACProjectile::OnHit()
{
	bHit = true;
	BP_OnHit();
}

void ACProjectile::Destroyed()
{

	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

void ACProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor))
	{
		return;
	}
	if (!bHit) OnHit();
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			UAbilitySystemBlueprintLibrary::EffectContextAddHitResult(DamageSpec.Data.Get()->GetEffectContext(), SweepResult, true);
			DamageSpec.Data.Get()->GetContext().GetInstigatorAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), TargetASC);
		}
		Destroy();
	}
	else bHit = true;
}

bool ACProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageSpec.Data.Get()->GetContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor() == OtherActor|| !UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		return false;
	}
	return true;
}


