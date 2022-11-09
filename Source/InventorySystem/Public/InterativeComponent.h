// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InterativeComponent.generated.h"


class IInterativeInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInterativeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInterativeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual AActor* Rule(TArray<AActor*> InterativeActors);

	virtual void BeginTrigger();
	virtual void EndTrigger();

	UPROPERTY(EditAnywhere)
		float TraceRadius;

private:
	AActor* NowInterativeActor;

	bool bInInterative;
};
