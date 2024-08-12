// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CItem.h"

// Sets default values
ACItem::ACItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

