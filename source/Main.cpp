#include "plugin.h"
#include "MemoryMgr.h"
#include "CCamera.h"
void	HighspeedCamShake(float shake);
void	CamShakeFunction(float shake);
//#define REPLACEBLURWITHCAMSHAKE // When this is defined, blur gets replaced with camera shake.

WRAPPER void CamShakeNoPos(CCamera* pCamera, float fStrength) { WRAPARG(pCamera); WRAPARG(fStrength); EAXJMP(0x50A970); }

void PerformCamShake() {
CVehicle* vehicle = FindPlayerVehicle(-1, true);
#ifdef REPLACEBLURWITHCAMSHAKE
InjectHook(0x704E8A, &HighspeedCamShake);
InjectHook(0x704E8A, &CamShakeFunction);
#else
	if (vehicle && vehicle->m_nVehicleSubClass != VEHICLE_PLANE && vehicle->m_nVehicleSubClass != VEHICLE_HELI
		&& vehicle->m_nVehicleSubClass != VEHICLE_BOAT && vehicle->m_nVehicleSubClass != VEHICLE_BMX &&
		vehicle->m_nVehicleSubClass != VEHICLE_TRAIN) {
		float speed = vehicle->m_vecMoveSpeed.Magnitude();

		if (speed > 0.75f)
			CamShakeNoPos(&TheCamera, FindPlayerVehicle(-1, 0)->m_vecMoveSpeed.Magnitude() * 0.040f);

	}
#endif
}

void HighspeedCamShake(float shake)
{
	CamShakeNoPos(&TheCamera, shake * 0.030f);
}

void CamShakeFunction(float fShake) {
	CamShakeNoPos(&TheCamera, fShake * 0.030f);
}

class CameraShake {
public:
	CameraShake() {
		plugin::Events::gameProcessEvent += []() {
			PerformCamShake();
		};
	}
} _CameraShake;
