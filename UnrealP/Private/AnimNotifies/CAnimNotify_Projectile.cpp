// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CAnimNotify_Projectile.h"
#include "Interface/ICombatInterface.h"
#include "Components/CCombatComponent.h"
void UCAnimNotify_Projectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	IICombatInterface* CombatInterface = Cast<IICombatInterface>(MeshComp->GetOwner());
	if (CombatInterface)
	{
		CombatInterface->GetCombatComponent()->ProjectileFire();
	}

}
