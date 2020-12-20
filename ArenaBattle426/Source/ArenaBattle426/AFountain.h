// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h" // ���� Ŭ�����Ǽ����� ��Ƶ� ��� CoreMinimal.h ���
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
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* water;
	
	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* light;
	
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* splash;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 id;
};
