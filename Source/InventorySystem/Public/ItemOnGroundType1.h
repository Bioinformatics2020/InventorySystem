// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InterativeInterface.h"
#include "ItemOnGroundType1.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AItemOnGroundType1 : public AActor, public IInterativeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemOnGroundType1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Init(int ItemID,int ItemNum, FName ItemIndex);

	UFUNCTION()
		void EndPickUp(int ItemNum, FName ItemIndex);
	UFUNCTION()
		void OnHitWorldStatic(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginReadyInterative_Implementation(AActor* Actor) override;
	virtual void EndReadyInterative_Implementation(AActor* Actor) override;
	virtual void TickReadyInterative_Implementation(AActor* Actor, float DeltaTime) override;

	virtual void BeginInterative_Implementation(AActor* Actor, UInterativeComponent* NewInterativeComponent) override;
	virtual void EndInterative_Implementation(AActor* Actor) override;
	virtual void TickInterative_Implementation(AActor* Actor, float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* TipBorderWidgetComp;

	UInterativeComponent* InterativeComponent;
	int ID;
	int Num;
	FName Index;

	bool bEndPickUp;
	bool bHadWidget;
	struct FItemOnGroundAttr* ItemOnGroundAttr;
};
