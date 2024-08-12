// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFootAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector LeftDistance = {0,0,0};

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector RightDistance = { 0,0,0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector PelvisDistance = { 0,0,0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator LeftRotation = { 0,0,0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FRotator RightRotation = { 0,0,0 };
};

UCLASS()
class UNREALP_API UCFootAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	FName LeftSocket = "Foot_L";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	FName RightSocket = "Foot_R";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	float TraceDistance = 55;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	float InterpSpeed = 17;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	float OffsetDistance = 5;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation);
private:
	class ACharacter* OwnerCharacter;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InverseKinemetics")
	FFeetData Data;
};
