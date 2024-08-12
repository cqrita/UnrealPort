// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CGameplayAbility_Damage.h"
#include "TimerManager.h"
#include "Interface/IComboInterface.h"
#include "CGameplayAbility_Melee.generated.h"

class AActor;
class UCCombatComponent;
class UAnimMontage;

USTRUCT()
struct FVector2DArray {
	GENERATED_BODY()
public:

	TArray<FVector> Ar;

	FVector operator[] (int32 i) {
		return Ar[i];
	}

	void Add(FVector Vector) {
		Ar.Add(Vector);
	}
	void Push(FVector Vector) {
		Ar.Push(Vector);
	}
	void Empty() {
		Ar.Empty();
	}
	int Num() {
		return Ar.Num();
	}
};

/**
 * 
 */
UCLASS()
class UNREALP_API UCGameplayAbility_Melee : public UCGameplayAbility_Damage,public IIComboInterface
{
	GENERATED_BODY()
public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
protected:
	UPROPERTY()
	TArray<AActor*> HitArray;
protected:
	UFUNCTION(BlueprintCallable)
	bool CheckIfHit(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	void ClearHitArray();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bComboFlag=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentComboNum=0;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ComboCall();

protected:
	UPROPERTY()
	FTimerHandle CollisionTimerHandle;
	UPROPERTY()
	TArray<FVector2DArray> CurrentSamples;
	UPROPERTY()
	TArray<FVector2DArray> PreviousSamples;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int SampleNumbers=10;
public:
	UFUNCTION(BlueprintCallable)
	void CollisionEnabled();
	UFUNCTION(BlueprintCallable)
	void CollisionDisabled();
	UFUNCTION(BlueprintCallable)
	void CheckCollision();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCollisionTraced(FHitResult OtherActor);
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void MakeTransientField(FVector Location);
public:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetBaseAttackMontage() const;
	UFUNCTION(BlueprintCallable)
	int GetMaxComboNum() const;
	UFUNCTION(BlueprintCallable)
	float GetAttackSpeed() const;
};
