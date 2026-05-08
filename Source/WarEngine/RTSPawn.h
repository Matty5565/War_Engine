#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSPawn.generated.h"

class UCameraComponent;
class ACesiumGeoreference;

UCLASS()
class WARENGINE_API ARTSPawn : public APawn
{
	GENERATED_BODY()

public:
	ARTSPawn();

	// Altitude clamps in metres
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float MinAltitudeMeters = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float MaxAltitudeMeters = 15000.f;

	// Pan speed: cm/s per metre of current altitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float PanSpeedBase = 8.f;

	// Metres of altitude change per scroll tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float ZoomStepMeters = 600.f;

	// Pixels from viewport edge that trigger edge scroll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float EdgeScrollMarginPx = 20.f;

	// Speed multiplier for edge scroll relative to WASD (0.5 = half speed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Camera")
	float EdgeScrollSpeed = 0.3f;

	// Normandy bbox — edit in Details to change play area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Bounds")
	double MinLat =  48.454709;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Bounds")
	double MaxLat =  49.768848;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Bounds")
	double MinLon =  -2.084656;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RTS|Bounds")
	double MaxLon =   1.408997;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	ACesiumGeoreference* GeoRef = nullptr;

	FVector2D PanInput   = FVector2D::ZeroVector;
	float     ZoomInput  = 0.f;

	UFUNCTION() void OnScrollUp()   { ZoomInput += 1.f; }
	UFUNCTION() void OnScrollDown() { ZoomInput -= 1.f; }

	void GatherKeyInput();
	void HandleEdgeScroll();
	void ApplyPanAndZoom(float DeltaTime);
	void ClampToBounds();
};
