// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOnGroundType1.h"
#include "InventorySystem/DataAssetMananger/DataAssetMananger.h"
#include "DataTableToolLibrary.h"
#include "StructTypes.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/WidgetComponent.h>
#include "InventorySystem/EventManager/EventManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AItemOnGroundType1::AItemOnGroundType1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComponent"));
	TipBorderWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("TipBorderWidget"));
	SetRootComponent(SphereComponent);
	MeshComponent->AttachToComponent(SphereComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	SphereComponent->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TipBorderWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	TipBorderWidgetComp->SetDrawAtDesiredSize(true);
	TipBorderWidgetComp->AttachToComponent(SphereComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	FBodyInstance* BodyInstance = SphereComponent->GetBodyInstance();
	BodyInstance->bLockXRotation = true;
	BodyInstance->bLockYRotation = true;

	FEventManager::RegistEvent(FName("EndPickUp"), this, FName("EndPickUp"));
	SphereComponent->OnComponentHit.AddDynamic(this, &AItemOnGroundType1::OnHitWorldStatic);
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AItemOnGroundType1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemOnGroundType1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemOnGroundType1::Init(int ItemID,int ItemNum,FName ItemIndex)
{
	ItemOnGroundAttr = UDataTableToolLibrary::GetItemOnGroundAttr(FName(FString::FromInt(ItemID)));
	
	ID = ItemID;
	Num = ItemNum;
	Index = ItemIndex;
	bEndPickUp = false;
	bHadWidget = false;
	ADataAssetMananger::RequestAsyncLoadObject(this, ItemOnGroundAttr->Mesh, [this](UObject* Object) {
		MeshComponent->SetStaticMesh(Cast<UStaticMesh>(Object));
	});
}

void AItemOnGroundType1::EndPickUp(int ItemNum, FName ItemIndex)
{
	if (ItemIndex == Index)
	{
		Num = ItemNum;
		SphereComponent->SetSimulatePhysics(true);
		if (ItemNum != 0)
		{
			SphereComponent->AddImpulse(FVector(100, 0, 100), NAME_None, true);
			bEndPickUp = true;
		}

		if (IsValid(InterativeComponent))
		{
			InterativeComponent->EndTrigger();
		}

		if (ItemNum == 0)
		{
			FEventManager::UnRegistEvent(FName("EndPickUp"), this);

			UFunction* InitFunc = TipBorderWidgetComp->GetWidget()->FindFunction(FName("UpData"));
			checkf(InitFunc, TEXT("No Find Function:UpData "));
			TipBorderWidgetComp->GetWidget()->ProcessEvent(InitFunc, &Num);

			Destroy();
		}
	}
}

void AItemOnGroundType1::OnHitWorldStatic(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bEndPickUp == true)
	{
		SphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Overlap);
		bEndPickUp = false;
		struct 
		{
			FName Index;
			FVector Location;
		}Params;
		Params.Index = Index;
		Params.Location = GetActorLocation();
		FEventManager::TriggerEvent(FName("OverPickUpItemOnGround"), &Params);
	}
}

void AItemOnGroundType1::BeginReadyInterative_Implementation(AActor* Actor)
{
	if (!bHadWidget)
	{
		bHadWidget = true;
		UClass* TipBorderWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundAttr->TipBorderWidgetType);
		UClass* TipContextWidgetClass = ADataAssetMananger::RequestSyncLoadClass(this, ItemOnGroundAttr->TipContextWidgetType);
		UUserWidget* TipBorderWidget = CreateWidget(GetWorld(), TipBorderWidgetClass);
		UUserWidget* TipContextWidget = CreateWidget(GetWorld(), TipContextWidgetClass);

		UFunction* InitFunc = TipBorderWidget->FindFunction(FName("Init"));
		checkf(InitFunc, TEXT("No Find Function:Init "));
		struct
		{
			int ID;
			int Num;
			UWidget* ContextWidget;
		}Params;
		Params.ID = ID;
		Params.Num = Num;
		Params.ContextWidget = TipContextWidget;
		TipBorderWidget->ProcessEvent(InitFunc, &Params);

		TipBorderWidgetComp->SetWidget(TipBorderWidget);

	}
	TipBorderWidgetComp->SetVisibility(true);
}

void AItemOnGroundType1::EndReadyInterative_Implementation(AActor* Actor)
{
	TipBorderWidgetComp->SetVisibility(false);
}

void AItemOnGroundType1::TickReadyInterative_Implementation(AActor* Actor, float DeltaTime)
{
	MeshComponent->AddRelativeRotation(FRotator(0, 1, 0));
	FRotator CameraRatation = UGameplayStatics::GetPlayerController(this, 0)->PlayerCameraManager->GetCameraRotation();
	CameraRatation.Yaw += 180;
	CameraRatation.Pitch *= -1;
	TipBorderWidgetComp->SetWorldRotation(CameraRatation);
}

void AItemOnGroundType1::BeginInterative_Implementation(AActor* Actor, UInterativeComponent* NewInterativeComponent)
{
	SphereComponent->SetSimulatePhysics(false);
	InterativeComponent = NewInterativeComponent;
}

void AItemOnGroundType1::EndInterative_Implementation(AActor* Actor)
{
	
	SphereComponent->SetSimulatePhysics(true);
	InterativeComponent = NULL;
}

void AItemOnGroundType1::TickInterative_Implementation(AActor* Actor, float DeltaTime)
{
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), Actor->GetActorLocation(), DeltaTime, 5));

	if (FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation())<400)
	{
		FEventManager::TriggerEvent(FName("TryPickUpItemOnGround"), &Index);
		SphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
}

