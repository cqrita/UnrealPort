// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CParkourComponent.generated.h"

class ACharacter;
class UArrowComponent;
class USceneComponent;
UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	ECenter = 0, ECeil, EFloor, ELand, EMax,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	EClimb = 0, EFall, ESlide, ENormal, EMax
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCParkourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE bool IsExecuting() { return Type != EParkourType::EMax; }

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 150;
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere)
	float ClimbExtent = 300;

	UPROPERTY(EditAnywhere)
	float FallDistance = 600;
	UPROPERTY(EditAnywhere)
	float FallExtent = 600;

	UPROPERTY(EditAnywhere)
	float ObstacleExtent = 400;
	UPROPERTY(EditAnywhere)
	float ObstacleOffset= 350;

	UPROPERTY(EditAnywhere)
	float AvailableFrontAngle = 15;

private:
	void LineTrace(EParkourArrowType InType);

public:
	void CheckTrace_Center();
	void CheckTrace_Ceil();
	void CheckTrace_Floor();
	void CheckTrace_Land();

public:
	bool Check_Obstacle();

public:
	bool Check_ClimbMode();

public:
	bool Check_FallMode();

public:
	bool Check_ObstacleMode();

public:
	void InitializeParkour();

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> ArrowGroup;
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UArrowComponent> Arrows[(int32)EParkourArrowType::EMax];

public:
	UPROPERTY()
	FHitResult HitResults[(int32)EParkourArrowType::EMax];

public:
	UPROPERTY()
	AActor* HitObstacle;
	UPROPERTY()
	FVector HitObstacleExtent;
	UPROPERTY()
	float HitDistance;
	UPROPERTY()
	float ToFrontYaw;
public:
	FORCEINLINE AActor* GetHitObstacle() { return HitObstacle; }

private:
	UPROPERTY()
	EParkourType Type = EParkourType::EMax;
public:
	FORCEINLINE EParkourType GetParkourType() { return Type; }
	FORCEINLINE void SetParkourType(EParkourType InType) { Type=InType; }
private:
	UPROPERTY()
	bool bFalling;
};
