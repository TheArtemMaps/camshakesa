#include "plugin.h"
#include "MemoryMgr.h"
#include "CCamera.h"


WRAPPER void CamShakeNoPos(CCamera* pCamera, float fStrength) { WRAPARG(pCamera); WRAPARG(fStrength); EAXJMP(0x50A970); }

void PerformCamShake() {
	CVehicle* vehicle = FindPlayerVehicle(-1, true);

	if (vehicle && vehicle->m_nVehicleSubClass != VEHICLE_PLANE && vehicle->m_nVehicleSubClass != VEHICLE_HELI
		&& vehicle->m_nVehicleSubClass != VEHICLE_BOAT && vehicle->m_nVehicleSubClass != VEHICLE_BMX &&
		vehicle->m_nVehicleSubClass != VEHICLE_TRAIN) {
		float speed = vehicle->m_vecMoveSpeed.Magnitude();

		if (speed > 0.75f)
			CamShakeNoPos(&TheCamera, FindPlayerVehicle(-1, 0)->m_vecMoveSpeed.Magnitude() * 0.080f);
	}
}



class CameraShake {
public:
	CameraShake() {
		plugin::Events::gameProcessEvent += []() {
			PerformCamShake();
		};
	}
} _CameraShake;