// Fill out your copyright notice in the Description page of Project Settings.


#include "InterativeComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "InterativeInterface.h"

// Sets default values for this component's properties
UInterativeComponent::UInterativeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInterativeComponent::BeginPlay()
{
	Super::BeginPlay();
	TraceRadius = 100;
	// ...
	
}


// Called every frame
void UInterativeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bInInterative)
	{
		IInterativeInterface::Execute_TickInterative(NowInterativeActor, GetOwner(), DeltaTime);
	}

	TArray<AActor*> InterativeActors;
	TArray<FHitResult> HitResults;
	if (UKismetSystemLibrary::SphereTraceMulti(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + FVector(0, 0, 10), TraceRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, { GetOwner() }, EDrawDebugTrace::ForOneFrame, HitResults, true))
	{
		for (FHitResult& HitResult:HitResults)
		{
			
			if (HitResult.Actor->GetClass()->ImplementsInterface(UInterativeInterface::StaticClass()))
			{
				InterativeActors.Add(HitResult.Actor.Get());
			}
		}
	}
	AActor* NewInterativeActor = Rule(InterativeActors);

	if (NowInterativeActor != NULL)
	{
		if (NewInterativeActor == NowInterativeActor)
		{
			if (!bInInterative)
			{
				IInterativeInterface::Execute_TickReadyInterative(NewInterativeActor, GetOwner(), DeltaTime);
			}
		}
		else
		{
			EndTrigger();
			IInterativeInterface::Execute_EndReadyInterative(NowInterativeActor, GetOwner());
		}
	}
	if(NewInterativeActor != NULL && NewInterativeActor != NowInterativeActor)
	{
		IInterativeInterface::Execute_BeginReadyInterative(NewInterativeActor, GetOwner());

	}

	NowInterativeActor = NewInterativeActor;
}

AActor* UInterativeComponent::Rule(TArray<AActor*> InterativeActors)
{
	FVector Location = GetOwner()->GetActorLocation();
	float Distance = 1e10;
	AActor* NearActor=NULL;
	for (AActor* InterativeActor:InterativeActors)
	{
		FVector OtherLocation = InterativeActor->GetActorLocation() - Location;
		float TempDistance = OtherLocation.X*OtherLocation.X + OtherLocation.Y*OtherLocation.Y + OtherLocation.Z*OtherLocation.Z;
		if (TempDistance < Distance)
		{
			Distance = TempDistance;
			NearActor = InterativeActor;
		}
	}
	return NearActor;
}

void UInterativeComponent::BeginTrigger()
{
	if (!bInInterative && NowInterativeActor)
	{
		IInterativeInterface::Execute_BeginInterative(NowInterativeActor, GetOwner(), this);
		IInterativeInterface::Execute_TickInterative(NowInterativeActor, GetOwner(), 0);
		bInInterative = true;
	}
}

void UInterativeComponent::EndTrigger()
{
	if (bInInterative && IsValid(NowInterativeActor))
	{
		IInterativeInterface::Execute_EndInterative(NowInterativeActor, GetOwner());
		bInInterative = false;
	}
}

