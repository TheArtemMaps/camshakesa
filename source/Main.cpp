#include "plugin.h"
#include "MemoryMgr.h"
#include "CCamera.h"
#include "CGeneral.h"
void	CamShakeFunction(float shake);
#define REPLACEBLURWITHCAMSHAKE // When this is defined, blur gets replaced with camera shake.

void PerformCamShake() {
CVehicle* vehicle = FindPlayerVehicle(-1, true);
#ifdef REPLACEBLURWITHCAMSHAKE
InjectHook(0x704E8A, &CamShakeFunction);
#else
	if (vehicle && vehicle->m_nVehicleSubClass != VEHICLE_PLANE && vehicle->m_nVehicleSubClass != VEHICLE_HELI
		&& vehicle->m_nVehicleSubClass != VEHICLE_BOAT && vehicle->m_nVehicleSubClass != VEHICLE_BMX &&
		vehicle->m_nVehicleSubClass != VEHICLE_TRAIN) {
		float speed = vehicle->m_vecMoveSpeed.Magnitude();

		if (speed > 0.75f)
			CGeneral::CamShakeNoPos(&TheCamera, FindPlayerVehicle(-1, 0)->m_vecMoveSpeed.Magnitude() * 0.040f);

	}
#endif
}

void CamShakeFunction(float fShake) {
	CGeneral::CamShakeNoPos(&TheCamera, fShake * 0.035f);
}

class CameraShake {
public:
	CameraShake() {
		plugin::Events::gameProcessEvent += []() {
			PerformCamShake();
		};
	}
} _CameraShake;
