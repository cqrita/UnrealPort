// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/FCGlobalTags.h"
#include "AbilitySystem/CAttributeSet.h"
FCGlobalTags FCGlobalTags::GlobalTags;

void FCGlobalTags::AddTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	DamageTag = Manager.AddNativeGameplayTag(FName(UCAttributeSet::GetDamageAttribute().AttributeName));

}
