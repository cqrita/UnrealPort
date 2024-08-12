// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/IStatusInterface.h"
#include "Components/CStatusComponent.h"
void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}
void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	IGenericTeamAgentInterface* OwnerPawn = Cast<IGenericTeamAgentInterface>(InPawn);
	SetGenericTeamId(OwnerPawn->GetGenericTeamId());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerception);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACAIController::OnPerceptionForgotten);
	UBlackboardComponent* BlackboardComp = Blackboard;
	UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp);
	RunBehaviorTree(BehaviorTree);
	IIStatusInterface* StatusInterface = Cast<IIStatusInterface>(InPawn);
	if (StatusInterface)
	{
		StatusComponent = StatusInterface->GetStatusComponent();
	}
}
void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	PerceptionComponent->OnPerceptionUpdated.Clear();
}
void ACAIController::OnPerception(AActor* TargetActor, FAIStimulus Stimulus)
{
	TArray<AActor*> Threats;
	PerceptionComponent->GetHostileActors(Threats);
	if (Threats.Num() <= 0)
	{
		return;
	}
	const int32 i = Threats.Find(TargetActor);
	if (i < 0)
	{
		return;
	}
	IIStatusInterface* TargetStatusInterface = Cast<IIStatusInterface>(TargetActor);
	if (!TargetStatusInterface)
	{
		return;
	}
	if (!TargetStatusInterface->GetIsAlive())
	{
		return;
	}
	Target = TargetActor;
	SetFocus(TargetActor);
}
void ACAIController::OnPerceptionForgotten(AActor* TargetActor)
{
	Target = nullptr;
	ClearFocus(EAIFocusPriority::Gameplay);
}
ACAIController::ACAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	Behavior = CreateDefaultSubobject<UCBehaviorComponent>("BehaviorComponent");
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Damage = CreateDefaultSubobject<UAISenseConfig_Damage>("Damage");
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>("BehaviorTree");
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	Sight->SightRadius = 1600;
	Sight->LoseSightRadius = 1000;
	Sight->PeripheralVisionAngleDegrees = 45;
	Sight->SetMaxAge(0.01);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	PerceptionComponent->ConfigureSense(*Sight);
	Damage->SetMaxAge(1);
	PerceptionComponent->ConfigureSense(*Damage);
	PerceptionComponent->SetDominantSense(*Sight->GetSenseImplementation());

}

void ACAIController::Tick(float DeltaTime)
{
	if (StatusComponent->GetIsAlive())
	{
		Super::Tick(DeltaTime);
	}
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

ETeamAttitude::Type ACAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	auto TeamPawn= Cast<IGenericTeamAgentInterface>(OtherPawn);
	if (TeamPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	FGenericTeamId TeamId = GetGenericTeamId();
	FGenericTeamId OtherTeamId = TeamPawn->GetGenericTeamId();
	if (TeamId == OtherTeamId)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}

EBehaviorType ACAIController::GetBehavior()
{
	if (!Behavior)
	{
		return EBehaviorType::EWait;
	}
	return Behavior->GetBehavior();
}

void ACAIController::SetBehavior(EBehaviorType InBehavior)
{
	if (!Behavior)
	{
		return;
	}
	return Behavior->SetBehavior(InBehavior);
}

void ACAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, false);

	//Smooth and change the pawn rotation
	if (bUpdatePawn)
	{
		//Get pawn

		APawn* const MyPawn = GetPawn();
		//Get Pawn current rotation
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		//Calculate smoothed rotation
		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);
		//Check if we need to change
		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			//Change rotation using the Smooth Target Rotation
			MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}

}
