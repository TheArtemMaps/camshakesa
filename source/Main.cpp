#include "plugin.h"
#include "MemoryMgr.h"
#include "CCamera.h"
#include "CGeneral.h"
#include "ini.h"
void	CamShakeFunction(float shake);
float ShakeIntensity;
float RequiredSpeed;
//#define REPLACEBLURWITHCAMSHAKE // When this is defined, blur gets replaced with camera shake.

void PerformCamShake() {
#ifdef REPLACEBLURWITHCAMSHAKE
InjectHook(0x704E8A, &CamShakeFunction);
#else
	CVehicle* vehicle = FindPlayerVehicle(-1, true);
	if (vehicle && vehicle->m_nVehicleSubClass != VEHICLE_PLANE && vehicle->m_nVehicleSubClass != VEHICLE_HELI
		&& vehicle->m_nVehicleSubClass != VEHICLE_BOAT && vehicle->m_nVehicleSubClass != VEHICLE_BMX &&
		vehicle->m_nVehicleSubClass != VEHICLE_TRAIN) {
		float speed = vehicle->m_vecMoveSpeed.Magnitude();
		if (speed > RequiredSpeed)
			CGeneral::CamShakeNoPos(&TheCamera, FindPlayerVehicle(-1, 0)->m_vecMoveSpeed.Magnitude() * ShakeIntensity);

	}
#endif
}

void CamShakeFunction(float fShake) {
	CGeneral::CamShakeNoPos(&TheCamera, ShakeIntensity);
}

void Init() {
	CVehicle* vehicle = FindPlayerVehicle(-1, true);
	mINI::INIFile file("camerashake.ini");
	mINI::INIStructure ini;
	file.read(ini);

	std::string shakeintensity = ini.get("MAIN").get("ShakeIntensity");
	const char* shakeintensity2 = shakeintensity.c_str();
	ShakeIntensity = std::atof(shakeintensity2);

	std::string requiredspeed = ini.get("MAIN").get("RequiredSpeed");
	const char* requiredspeed2 = requiredspeed.c_str();
	RequiredSpeed = std::atof(requiredspeed2);
}



class CameraShake {
public:
	CameraShake() {
		plugin::Events::processScriptsEvent += []() {
			PerformCamShake();
		};
		plugin::Events::initGameEvent += []() {
			Init();
		};
	}
} _CameraShake;