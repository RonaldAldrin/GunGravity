// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GravityHUD.generated.h"

/**
 * 
 */
UCLASS()
class GRAVITYGUNTEST_API AGravityHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TObjectPtr<UUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

public:

	void InitOverlayWidget();
};
