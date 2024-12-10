// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityDetailStruct.generated.h"


USTRUCT(BlueprintType)
struct FAbilityDetailInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Cooldown = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
};



/**
 * 
 */
UCLASS()
class GRAVITYGUNTEST_API UAbilityDetailStruct : public UDataAsset
{
	GENERATED_BODY()
	
};
