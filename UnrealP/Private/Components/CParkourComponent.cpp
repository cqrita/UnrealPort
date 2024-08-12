// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CParkourComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UCParkourComponent::UCParkourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	ArrowGroup = CreateDefaultSubobject<USceneComponent>("ArrowGroup");
	for (int32 i = 0; i < (int32)EParkourArrowType::EMax; i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		Arrows[i] = CreateDefaultSubobject<UArrowComponent>(FName(name));
		//Arrows[i]->SetupAttachment(ArrowGroup);
		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::ECenter:
			Arrows[i]->ArrowColor = FColor::Red;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -60));
			break;

		case EParkourArrowType::ECeil:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;

		case EParkourArrowType::EFloor:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;

		case EParkourArrowType::ELand:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}
}
// Called when the game starts
void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	// ...
	
	//ArrowGroup->AttachToComponent(OwnerCharacter->GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true));
	for (int32 i = 0; i < (int32)EParkourArrowType::EMax; i++)
	{
		Arrows[i]->AttachToComponent(OwnerCharacter->GetCapsuleComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	}
}


// Called every frame
void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (OwnerCharacter->GetCharacterMovement())
	{
		CheckTrace_Land();
	}
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), false, ignores, DebugType, HitResults[(int32)InType], true, color, FLinearColor::White);
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::ECenter;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	if (hitResult.bBlockingHit==false)
	{
		return;
	}

	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(hitResult.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (mesh == nullptr)
	{
		return;
	}


	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z)* mesh->GetRelativeScale3D();

	HitDistance = hitResult.Distance;

	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;
}

void UCParkourComponent::CheckTrace_Ceil()
{
	LineTrace(EParkourArrowType::ECeil);
}

void UCParkourComponent::CheckTrace_Floor()
{
	LineTrace(EParkourArrowType::EFloor);
}

void UCParkourComponent::CheckTrace_Land()
{
	if (OwnerCharacter->GetCharacterMovement()->IsFalling() == false)
	{
		return;
	}
	bFalling = true;

	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::ELand];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();

	FVector end = start + transform.GetRotation().GetForwardVector() * FallExtent;

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	HitResults[(int32)EParkourArrowType::ELand] = FHitResult();
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignores, DebugType, HitResults[(int32)EParkourArrowType::ELand], true, color, FLinearColor::White);
}

bool UCParkourComponent::Check_Obstacle()
{
	if (HitObstacle == nullptr)
	{
		return false;
	}
	bool b = HitResults[(int32)EParkourArrowType::ECenter].bBlockingHit;
	if (b == false)
	{
		return false;
	}

	FVector start = HitResults[(int32)EParkourArrowType::ECenter].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;


	FVector impactNormal = HitResults[(int32)EParkourArrowType::ECenter].ImpactNormal;
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float yaw = abs(abs(lookAt) - abs(impactAt));
	if (!(yaw <= AvailableFrontAngle))
	{
		return false;
	}

	return true;
}


bool UCParkourComponent::Check_ClimbMode()
{
	if (HitResults[(int32)EParkourArrowType::ECeil].bBlockingHit == false)
	{
		return false;
	}
	if (ClimbExtent > HitObstacleExtent.Z)
	{
		return false;
	}
	return true;
}

bool UCParkourComponent::Check_FallMode()
{
	if (bFalling == false)
	{
		HitResults[(int32)EParkourArrowType::ELand] = FHitResult();
		return false;
	}
	bFalling = false;

	float distance = HitResults[(int32)EParkourArrowType::ELand].Distance;
	if (FallDistance > distance)
	{
		HitResults[(int32)EParkourArrowType::ELand] = FHitResult();
		return false;
	}
	HitResults[(int32)EParkourArrowType::ELand] = FHitResult();
	return true;
}

bool UCParkourComponent::Check_ObstacleMode()
{
	if (HitResults[(int32)EParkourArrowType::ECeil].bBlockingHit == true)
	{
		return false;
	}
	bool b = FMath::IsNearlyEqual(ObstacleExtent, HitObstacleExtent.Y, ObstacleOffset);
	if (b == true)
	{
		return true;
	}
	return false;
}

void UCParkourComponent::InitializeParkour()
{
	HitObstacle = NULL;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0;
	ToFrontYaw = 0;
}