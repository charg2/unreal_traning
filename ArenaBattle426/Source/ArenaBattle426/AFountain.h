// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle426.h" // 엔진 클래스의선언을 모아둔 헤더 CoreMinimal.h 대신
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "AFountain.generated.h"

UCLASS()
class ARENABATTLE426_API AAFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;
	virtual void PostInitializeComponents() override final;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override final;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;
	
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;


private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta=(AllowPrivateAccess=true))
	float RotateSpeed;

};
