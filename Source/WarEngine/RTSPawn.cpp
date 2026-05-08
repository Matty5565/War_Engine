#include "RTSPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "CesiumGeoreference.h"

ARTSPawn::ARTSPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Camera);
	Camera->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // True top-down

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;
}

void ARTSPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputMode);
	}

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACesiumGeoreference::StaticClass(), Found);
	if (Found.Num() > 0)
		GeoRef = Cast<ACesiumGeoreference>(Found[0]);
}

void ARTSPawn::SetupPlayerInputComponent(UInputComponent* IC)
{
	Super::SetupPlayerInputComponent(IC);
	// Scroll wheel — fires once per tick regardless of input system
	IC->BindKey(EKeys::MouseScrollUp,   IE_Pressed, this, &ARTSPawn::OnScrollUp);
	IC->BindKey(EKeys::MouseScrollDown, IE_Pressed, this, &ARTSPawn::OnScrollDown);
}

void ARTSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GatherKeyInput();
	HandleEdgeScroll();
	ApplyPanAndZoom(DeltaTime);

	PanInput  = FVector2D::ZeroVector;
	ZoomInput = 0.f;
}

void ARTSPawn::GatherKeyInput()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	// WASD + arrow keys — direct query, no binding assets needed
	if (PC->IsInputKeyDown(EKeys::W) || PC->IsInputKeyDown(EKeys::Up))    PanInput.Y += 1.f;
	if (PC->IsInputKeyDown(EKeys::S) || PC->IsInputKeyDown(EKeys::Down))  PanInput.Y -= 1.f;
	if (PC->IsInputKeyDown(EKeys::D) || PC->IsInputKeyDown(EKeys::Right))  PanInput.X += 1.f;
	if (PC->IsInputKeyDown(EKeys::A) || PC->IsInputKeyDown(EKeys::Left))   PanInput.X -= 1.f;
}

void ARTSPawn::HandleEdgeScroll()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	int32 VX = 0, VY = 0;
	float MX = 0.f, MY = 0.f;
	PC->GetViewportSize(VX, VY);
	if (!PC->GetMousePosition(MX, MY)) return;

	if (MX <= EdgeScrollMarginPx)            PanInput.X -= EdgeScrollSpeed;
	if (MX >= VX - EdgeScrollMarginPx)       PanInput.X += EdgeScrollSpeed;
	if (MY <= EdgeScrollMarginPx)            PanInput.Y += EdgeScrollSpeed;
	if (MY >= VY - EdgeScrollMarginPx)       PanInput.Y -= EdgeScrollSpeed;
}

void ARTSPawn::ApplyPanAndZoom(float DeltaTime)
{
	if (!GeoRef) return;

	FVector LLH = GeoRef->TransformUnrealPositionToLongitudeLatitudeHeight(GetActorLocation());

	// Zoom: discrete per scroll tick — no DeltaTime so each tick = fixed altitude step
	LLH.Z -= ZoomInput * ZoomStepMeters;
	LLH.Z  = FMath::Clamp(LLH.Z, (double)MinAltitudeMeters, (double)MaxAltitudeMeters);

	// Pan: speed scales with altitude so movement feels consistent at any zoom level
	// +X in UE world ≈ East, +Y ≈ North at the Cesium georeference origin
	float SpeedCmSec = PanSpeedBase * FMath::Max((float)LLH.Z, 1.f);
	FVector WorldDelta(
		PanInput.Y * SpeedCmSec * DeltaTime,  // Forward/back → North/South
		PanInput.X * SpeedCmSec * DeltaTime,  // Left/right   → West/East
		0.f
	);

	FVector NewPos = GeoRef->TransformLongitudeLatitudeHeightPositionToUnreal(LLH) + WorldDelta;
	SetActorLocation(NewPos);

	ClampToBounds();
}

void ARTSPawn::ClampToBounds()
{
	if (!GeoRef) return;

	FVector LLH  = GeoRef->TransformUnrealPositionToLongitudeLatitudeHeight(GetActorLocation());
	bool bClamped = false;

	auto Clamp = [&](double& V, double Lo, double Hi)
	{
		if (V < Lo) { V = Lo; bClamped = true; }
		if (V > Hi) { V = Hi; bClamped = true; }
	};

	Clamp(LLH.X, MinLon, MaxLon);
	Clamp(LLH.Y, MinLat, MaxLat);
	Clamp(LLH.Z, (double)MinAltitudeMeters, (double)MaxAltitudeMeters);

	if (bClamped)
		SetActorLocation(GeoRef->TransformLongitudeLatitudeHeightPositionToUnreal(LLH));
}
