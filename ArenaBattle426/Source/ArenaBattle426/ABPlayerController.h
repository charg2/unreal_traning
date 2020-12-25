// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle426.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE426_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void PostInitializeComponents() override final;
	virtual void OnPossess(APawn* aPawn) override final;

protected:
	virtual void BeginPlay() final;
};
