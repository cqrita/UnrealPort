// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/CAnimNotify_Unequip.h"
#include "Interface/IItemInterface.h"
#include "Components/CItemComponent.h"
void UCAnimNotify_Unequip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	IIItemInterface* ItemInterface = Cast<IIItemInterface>(MeshComp->GetOwner());
	if (ItemInterface)
	{
		ItemInterface->GetItemComponent()->BeginUnequip();
	}
}
