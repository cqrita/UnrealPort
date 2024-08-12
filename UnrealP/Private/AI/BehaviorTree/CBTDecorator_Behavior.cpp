// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviorTree/CBTDecorator_Behavior.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "Interface/IBehaviorInterface.h"
bool UCBTDecorator_Behavior::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner();
	IIBehaviorInterface* BehaviorInterface= Cast<IIBehaviorInterface>(AIController);
	if (BehaviorInterface->GetBehavior() == Behavior)
	{
		result= true;

	}
	else
	{
		result= false;

	}
	return result;
}
