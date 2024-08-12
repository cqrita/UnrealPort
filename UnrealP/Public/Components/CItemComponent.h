// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "CItemComponent.generated.h"

class ACEquipment;
class ACWeapon;
class ACCharacterBase;
class UGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetNewEquipment, ACEquipment*, NewItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSetNewWeapon, ACWeapon*, NewWeapon, TArray<TSubclassOf<UGameplayAbility>>, GrantedAblities);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetNewItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALP_API UCItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY()
	TObjectPtr<ACCharacterBase> OwnerCharacter;
public:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	TArray<ACEquipment*> Equipments;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ACItem*> Items;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<ACEquipment> MainEquipment;
public:
	UPROPERTY()
	bool bOnEquipState;
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetOnEquipState() const { return bOnEquipState; }
	UFUNCTION(BlueprintCallable)
	void EquipEquipment(ACEquipment* Equipment);
	UFUNCTION(BlueprintCallable)
	void BeginEquip();
	UFUNCTION(BlueprintCallable)
	void EndEquip();
	UFUNCTION()
	void BeginEquipParams(ACEquipment* Equipment);
	UFUNCTION()
	void EndEquipParams(ACEquipment* Equipment);

	UPROPERTY()
	TObjectPtr<ACEquipment> TempEquipment;
	UFUNCTION(BlueprintPure)
	FORCEINLINE ACEquipment* GetTempEquipment() const { return TempEquipment; }
	UPROPERTY()
	bool bNotifyFired;
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetNotifyFired() const { return bNotifyFired; }

	UFUNCTION(BlueprintCallable)
	void UnequipEquipment(ACEquipment* Equipment);
	UFUNCTION(BlueprintCallable)
	void BeginUnequip();
	UFUNCTION(BlueprintCallable)
	void EndUnequip();
	UFUNCTION()
	void BeginUnequipParams(ACEquipment* Equipment);
	UFUNCTION()
	void EndUnequipParams(ACEquipment* Equipment);

	UFUNCTION(BlueprintCallable)
	void AddItem(ACItem* Item);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(ACItem* Item);
	UFUNCTION(BlueprintCallable)
	TArray<ACItem*> GetItems() const;
public:
	UPROPERTY(BlueprintAssignable)
	FSetNewEquipment SetNewEquipment;
	UPROPERTY(BlueprintAssignable)
	FSetNewWeapon SetNewWeapon;;
	UPROPERTY(BlueprintAssignable)
	FSetNewItem SetNewItem;
};
