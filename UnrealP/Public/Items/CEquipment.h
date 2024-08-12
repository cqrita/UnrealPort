// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CItem.h"
#include "CEquipment.generated.h"

class UCEquipmentDataAsset;
/**
 * 
 */
UCLASS()
class UNREALP_API ACEquipment : public ACItem
{
	GENERATED_BODY()
public:
	ACEquipment();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
public:
	UFUNCTION(BlueprintCallable)
	void EquipEquipment(USkeletalMeshComponent* OwnerMesh);
	UFUNCTION(BlueprintCallable)
	void UnequipEquipment(USkeletalMeshComponent* OwnerMesh);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SheathSocketName;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCEquipmentDataAsset> EquipmentDataAsset;
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCEquipmentDataAsset* GetEquipmentDataAsset() { return EquipmentDataAsset; }

};
