// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GravityHUD.h"
#include "Blueprint/UserWidget.h"

void AGravityHUD::BeginPlay()
{
	Super::BeginPlay();

	InitOverlayWidget();
}

void AGravityHUD::InitOverlayWidget()
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_GravityHUD"));

	OverlayWidget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	OverlayWidget->AddToViewport();

}

