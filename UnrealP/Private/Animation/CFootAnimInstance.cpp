// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CFootAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
void UCFootAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
}

void UCFootAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	float LeftDistance;
	FRotator LeftRotation;
	Trace(LeftSocket, LeftDistance, LeftRotation);

	float RightDistance;
	FRotator RightRotation;

	Trace(RightSocket, RightDistance, RightRotation);

	float offset = FMath::Min(LeftDistance, RightDistance);

	//Global 연산
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaSeconds, InterpSpeed);

	//local 연산
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(
		Data.LeftDistance.X, (LeftDistance - offset), DeltaSeconds, InterpSpeed
	);

	//local 연산 - 축기준으로 완전히 반전되어 있기 때문에 마이너스를 붙여서 음수로 만든다.
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(
		Data.RightDistance.X, -(RightDistance - offset), DeltaSeconds, InterpSpeed
	);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, LeftRotation, DeltaSeconds, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, RightRotation, DeltaSeconds, InterpSpeed);

}

void UCFootAnimInstance::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.0f;
	if (!OwnerCharacter)
	{
		return;
	}

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end = FVector(location.X, location.Y, traceZ);

	//DrawDebugLine(GetWorld(), start, end, FColor::Green);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(), start, end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		true, ignoreActors,
		DrawDebugType, hitResult, true,
		FLinearColor::Green, FLinearColor::Red
	);

	if (!hitResult.IsValidBlockingHit())
	{
		return;
	}

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = OffsetDistance + length - TraceDistance;

	FVector normal = hitResult.ImpactNormal;
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}
