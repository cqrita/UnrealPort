// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CCharacterBase.h"
#include "CEnemy.generated.h"
class UCWidgetComponent;
class UCHealthWidget;
/**
 * 
 */
UCLASS()
class UNREALP_API ACEnemy : public ACCharacterBase
{
	GENERATED_BODY()
public:
	ACEnemy();
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCWidgetComponent> HpBar;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UCHealthWidget> WidgetClass;

private:
	virtual void InitAbilityActorInfo() override;

};
