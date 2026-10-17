#include "RelicPlayerCharacter.h"

#include "RelicCombatComponent.h"
#include "../RelicGameMode.h"
#include "../RelicObjective.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "InputCoreTypes.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ARelicPlayerCharacter::ARelicPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 540.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SteelMaterial(TEXT("/Engine/TemplateResources/MI_Template_BaseGray_02_Metal.MI_Template_BaseGray_02_Metal"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AccentMaterial(TEXT("/Engine/TemplateResources/MI_Template_BaseOrange_Metal.MI_Template_BaseOrange_Metal"));
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutBody"));
	BodyMesh->SetupAttachment(GetRootComponent());
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetRelativeLocation(FVector(0.f, 0.f, 4.f));
	BodyMesh->SetRelativeScale3D(FVector(0.42f, 0.34f, 0.72f));
	if (CubeMesh.Succeeded()) BodyMesh->SetStaticMesh(CubeMesh.Object);
	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutHead"));
	HeadMesh->SetupAttachment(GetRootComponent());
	HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeadMesh->SetRelativeLocation(FVector(0.f, 0.f, 73.f));
	HeadMesh->SetRelativeScale3D(FVector(0.31f, 0.31f, 0.34f));
	if (SphereMesh.Succeeded()) HeadMesh->SetStaticMesh(SphereMesh.Object);
	PackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutPack"));
	PackMesh->SetupAttachment(GetRootComponent());
	PackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PackMesh->SetRelativeLocation(FVector(-20.f, 0.f, 18.f));
	PackMesh->SetRelativeScale3D(FVector(0.20f, 0.30f, 0.42f));
	if (CubeMesh.Succeeded()) PackMesh->SetStaticMesh(CubeMesh.Object);
	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutBlade"));
	BladeMesh->SetupAttachment(GetRootComponent());
	BladeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BladeMesh->SetRelativeLocation(FVector(30.f, 37.f, 51.f));
	BladeMesh->SetRelativeRotation(FRotator(0.f, 0.f, -8.f));
	BladeMesh->SetRelativeScale3D(FVector(0.085f, 0.065f, 0.56f));
	if (CubeMesh.Succeeded()) BladeMesh->SetStaticMesh(CubeMesh.Object);
	if (SteelMaterial.Succeeded()) BladeMesh->SetMaterial(0, SteelMaterial.Object);

	SwordGuardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordCrossguard"));
	SwordGuardMesh->SetupAttachment(GetRootComponent());
	SwordGuardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordGuardMesh->SetRelativeLocation(FVector(30.f, 37.f, 21.f));
	SwordGuardMesh->SetRelativeScale3D(FVector(0.12f, 0.30f, 0.055f));
	if (CubeMesh.Succeeded()) SwordGuardMesh->SetStaticMesh(CubeMesh.Object);
	if (AccentMaterial.Succeeded()) SwordGuardMesh->SetMaterial(0, AccentMaterial.Object);

	SwordGripMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordGrip"));
	SwordGripMesh->SetupAttachment(GetRootComponent());
	SwordGripMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordGripMesh->SetRelativeLocation(FVector(30.f, 37.f, 5.f));
	SwordGripMesh->SetRelativeScale3D(FVector(0.055f, 0.055f, 0.18f));
	if (CylinderMesh.Succeeded()) SwordGripMesh->SetStaticMesh(CylinderMesh.Object);

	SwordPommelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordPommel"));
	SwordPommelMesh->SetupAttachment(GetRootComponent());
	SwordPommelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordPommelMesh->SetRelativeLocation(FVector(30.f, 37.f, -5.f));
	SwordPommelMesh->SetRelativeScale3D(FVector(0.085f, 0.085f, 0.07f));
	if (SphereMesh.Succeeded()) SwordPommelMesh->SetStaticMesh(SphereMesh.Object);
	if (AccentMaterial.Succeeded()) SwordPommelMesh->SetMaterial(0, AccentMaterial.Object);

	// Compact sidearm silhouette on the opposite hip, built from engine primitives
	// so it is available in a clean source checkout without external art assets.
	PistolBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolReceiver"));
	PistolBodyMesh->SetupAttachment(GetRootComponent());
	PistolBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PistolBodyMesh->SetRelativeLocation(FVector(26.f, -35.f, 17.f));
	PistolBodyMesh->SetRelativeScale3D(FVector(0.25f, 0.09f, 0.085f));
	if (CubeMesh.Succeeded()) PistolBodyMesh->SetStaticMesh(CubeMesh.Object);
	if (SteelMaterial.Succeeded()) PistolBodyMesh->SetMaterial(0, SteelMaterial.Object);

	PistolGripMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolGrip"));
	PistolGripMesh->SetupAttachment(GetRootComponent());
	PistolGripMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PistolGripMesh->SetRelativeLocation(FVector(22.f, -35.f, 5.f));
	PistolGripMesh->SetRelativeRotation(FRotator(0.f, 0.f, -12.f));
	PistolGripMesh->SetRelativeScale3D(FVector(0.095f, 0.075f, 0.17f));
	if (CubeMesh.Succeeded()) PistolGripMesh->SetStaticMesh(CubeMesh.Object);
	if (AccentMaterial.Succeeded()) PistolGripMesh->SetMaterial(0, AccentMaterial.Object);

	PistolBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolBarrel"));
	PistolBarrelMesh->SetupAttachment(GetRootComponent());
	PistolBarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PistolBarrelMesh->SetRelativeLocation(FVector(41.f, -35.f, 18.f));
	PistolBarrelMesh->SetRelativeScale3D(FVector(0.12f, 0.045f, 0.045f));
	if (CubeMesh.Succeeded()) PistolBarrelMesh->SetStaticMesh(CubeMesh.Object);
	if (SteelMaterial.Succeeded()) PistolBarrelMesh->SetMaterial(0, SteelMaterial.Object);

	PistolSightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolSight"));
	PistolSightMesh->SetupAttachment(GetRootComponent());
	PistolSightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PistolSightMesh->SetRelativeLocation(FVector(29.f, -35.f, 24.f));
	PistolSightMesh->SetRelativeScale3D(FVector(0.045f, 0.055f, 0.035f));
	if (CubeMesh.Succeeded()) PistolSightMesh->SetStaticMesh(CubeMesh.Object);
	if (AccentMaterial.Succeeded()) PistolSightMesh->SetMaterial(0, AccentMaterial.Object);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 360.f;
	CameraBoom->SocketOffset = FVector(0.f, 48.f, 62.f);
	CameraBoom->bUsePawnControlRotation = true;
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 68.f));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetActive(false);

	Combat = CreateDefaultSubobject<URelicCombatComponent>(TEXT("RelicCombat"));
}

void ARelicPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	if (GetActorLocation().Z < 90.f) SetActorLocation(FVector(0.f, 0.f, 118.f));
}

void ARelicPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARelicPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARelicPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, this, &ARelicPlayerCharacter::LightAttack);
	PlayerInputComponent->BindAction(TEXT("HeavyAttack"), IE_Pressed, this, &ARelicPlayerCharacter::HeavyAttack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, Combat.Get(), &URelicCombatComponent::Dodge);
	PlayerInputComponent->BindAction(TEXT("Parry"), IE_Pressed, Combat.Get(), &URelicCombatComponent::BeginParry);
	PlayerInputComponent->BindAction(TEXT("Parry"), IE_Released, Combat.Get(), &URelicCombatComponent::EndParry);
	PlayerInputComponent->BindAction(TEXT("FirePistol"), IE_Pressed, this, &ARelicPlayerCharacter::FirePistol);
	PlayerInputComponent->BindAction(TEXT("Overload"), IE_Pressed, this, &ARelicPlayerCharacter::TryOverload);
	PlayerInputComponent->BindAction(TEXT("ToggleCamera"), IE_Pressed, this, &ARelicPlayerCharacter::ToggleCamera);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ARelicPlayerCharacter::TryInteract);
	PlayerInputComponent->BindAction(TEXT("Choice1"), IE_Pressed, this, &ARelicPlayerCharacter::ChooseMeridian);
	PlayerInputComponent->BindAction(TEXT("Choice2"), IE_Pressed, this, &ARelicPlayerCharacter::ChooseMarches);
	PlayerInputComponent->BindAction(TEXT("Choice3"), IE_Pressed, this, &ARelicPlayerCharacter::ChooseContinuum);
}

void ARelicPlayerCharacter::MoveForward(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value)) return;
	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::X), Value);
}

void ARelicPlayerCharacter::MoveRight(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value)) return;
	const FRotator Yaw(0.f, Controller->GetControlRotation().Yaw, 0.f);
	AddMovementInput(FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y), Value);
}

void ARelicPlayerCharacter::LightAttack()
{
	if (bFirstPerson) { bFirstPersonShowsSword = true; UpdateWeaponPresentation(); }
	if (Combat) Combat->LightAttack();
}

void ARelicPlayerCharacter::HeavyAttack()
{
	if (bFirstPerson) { bFirstPersonShowsSword = true; UpdateWeaponPresentation(); }
	if (Combat) Combat->HeavyAttack();
}

void ARelicPlayerCharacter::FirePistol()
{
	if (bFirstPerson) { bFirstPersonShowsSword = false; UpdateWeaponPresentation(); }
	if (Combat) Combat->FirePistol();
}

void ARelicPlayerCharacter::UpdateWeaponPresentation()
{
	USceneComponent* WeaponParent = bFirstPerson ? Cast<USceneComponent>(FirstPersonCamera) : GetRootComponent();
	const FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
	UStaticMeshComponent* WeaponParts[] = { BladeMesh, SwordGuardMesh, SwordGripMesh, SwordPommelMesh,
		PistolBodyMesh, PistolGripMesh, PistolBarrelMesh, PistolSightMesh };
	for (UStaticMeshComponent* Part : WeaponParts)
	{
		if (Part && Part->GetAttachParent() != WeaponParent) Part->AttachToComponent(WeaponParent, AttachRules);
	}

	if (bFirstPerson)
	{
		BladeMesh->SetRelativeLocation(FVector(78.f, 27.f, -24.f));
		BladeMesh->SetRelativeRotation(FRotator(0.f, 0.f, -4.f));
		SwordGuardMesh->SetRelativeLocation(FVector(77.f, 27.f, -54.f));
		SwordGripMesh->SetRelativeLocation(FVector(76.f, 27.f, -71.f));
		SwordPommelMesh->SetRelativeLocation(FVector(76.f, 27.f, -82.f));
		PistolBodyMesh->SetRelativeLocation(FVector(69.f, 24.f, -30.f));
		PistolGripMesh->SetRelativeLocation(FVector(65.f, 24.f, -43.f));
		PistolBarrelMesh->SetRelativeLocation(FVector(86.f, 24.f, -30.f));
		PistolSightMesh->SetRelativeLocation(FVector(72.f, 24.f, -23.f));
	}
	else
	{
		BladeMesh->SetRelativeLocation(FVector(30.f, 37.f, 51.f));
		BladeMesh->SetRelativeRotation(FRotator(0.f, 0.f, -8.f));
		SwordGuardMesh->SetRelativeLocation(FVector(30.f, 37.f, 21.f));
		SwordGripMesh->SetRelativeLocation(FVector(30.f, 37.f, 5.f));
		SwordPommelMesh->SetRelativeLocation(FVector(30.f, 37.f, -5.f));
		PistolBodyMesh->SetRelativeLocation(FVector(26.f, -35.f, 17.f));
		PistolGripMesh->SetRelativeLocation(FVector(22.f, -35.f, 5.f));
		PistolGripMesh->SetRelativeRotation(FRotator(0.f, 0.f, -12.f));
		PistolBarrelMesh->SetRelativeLocation(FVector(41.f, -35.f, 18.f));
		PistolSightMesh->SetRelativeLocation(FVector(29.f, -35.f, 24.f));
	}

	const bool bShowSword = !bFirstPerson || bFirstPersonShowsSword;
	const bool bShowPistol = !bFirstPerson || !bFirstPersonShowsSword;
	BladeMesh->SetVisibility(bShowSword, true);
	SwordGuardMesh->SetVisibility(bShowSword, true);
	SwordGripMesh->SetVisibility(bShowSword, true);
	SwordPommelMesh->SetVisibility(bShowSword, true);
	PistolBodyMesh->SetVisibility(bShowPistol, true);
	PistolGripMesh->SetVisibility(bShowPistol, true);
	PistolBarrelMesh->SetVisibility(bShowPistol, true);
	PistolSightMesh->SetVisibility(bShowPistol, true);
}

void ARelicPlayerCharacter::ToggleCamera()
{
	bFirstPerson = !bFirstPerson;
	ThirdPersonCamera->SetActive(!bFirstPerson);
	FirstPersonCamera->SetActive(bFirstPerson);
	HeadMesh->SetVisibility(!bFirstPerson, true);
	BodyMesh->SetVisibility(!bFirstPerson, true);
	PackMesh->SetVisibility(!bFirstPerson, true);
	UpdateWeaponPresentation();
	bUseControllerRotationYaw = bFirstPerson;
	GetCharacterMovement()->bOrientRotationToMovement = !bFirstPerson;
}

void ARelicPlayerCharacter::TryOverload()
{
	if (!Controller || !Combat) return;
	FVector Start;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Start, Rotation);
	const FVector End = Start + Rotation.Vector() * 1800.f;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RelicHackTarget), false, this);
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.GetActor())
	{
		Combat->Overload(Hit.GetActor());
	}
}

void ARelicPlayerCharacter::TryInteract()
{
	if (!Controller) return;
	FVector Start;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Start, Rotation);
	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RelicInteract), false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, Start + Rotation.Vector() * 350.f, ECC_Visibility, Params))
	{
		if (ARelicObjective* Objective = Cast<ARelicObjective>(Hit.GetActor())) Objective->Interact();
	}
}

void ARelicPlayerCharacter::ChooseMeridian() { if (ARelicGameMode* GM = Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode())) GM->SelectEnding(1); }
void ARelicPlayerCharacter::ChooseMarches() { if (ARelicGameMode* GM = Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode())) GM->SelectEnding(2); }
void ARelicPlayerCharacter::ChooseContinuum() { if (ARelicGameMode* GM = Cast<ARelicGameMode>(GetWorld()->GetAuthGameMode())) GM->SelectEnding(3); }

float ARelicPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Combat && Combat->GetCombatState() == ERelicCombatState::Parrying)
	{
		Combat->NotifyParrySucceeded(DamageCauser);
		return 0.f;
	}
	const float Applied = FMath::Min(Health, DamageAmount);
	Health = FMath::Max(0.f, Health - Applied);
	if (Health <= 0.f)
	{
		Health = MaxHealth;
		SetActorLocation(FVector(0.f, 0.f, 118.f));
	}
	return Applied;
}
