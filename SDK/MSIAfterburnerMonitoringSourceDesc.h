/////////////////////////////////////////////////////////////////////////////
//
// This header file defines hardware monitoring data source descriptor 
// stucture, used by hardware monitoring plugin modules to describe the
// sources, exported by the plugin
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _MONITORING_SOURCE_DESC_INCLUDED_
#define _MONITORING_SOURCE_DESC_INCLUDED_
/////////////////////////////////////////////////////////////////////////////
// v1.0 descriptor
/////////////////////////////////////////////////////////////////////////////
typedef struct MONITORING_SOURCE_DESC
{
	DWORD	dwVersion;
		//descriptor version ((major<<16) + minor)
		//must be set to 0x00010000 or greater by host to use this structure

		//Don't change this field when filling the descriptor!

	char	szName[MAX_PATH];
		//data source name (e.g. "GPU temperature")
	char	szUnits[MAX_PATH];
		//data source units (e.g. "°C")
	char	szFormat[MAX_PATH];
		//optional output format, may be empty to specify default %.0f format
	char	szGroup[MAX_PATH];
		//data source group name used for grouping data by it in OSD, Logitech keyboard LCD etc

	DWORD	dwID;
		//data source ID, MONITORING_SOURCE_ID_... 
	DWORD	dwInstance;
		//zero based data source instance index, e.g. 0 for "GPU1 temperature" in multiGPU system, 1 for "GPU2 temperature" in multiGPU system etc

	FLOAT	fltMaxLimit;
		//default maximum graph limit (e.g. 100°C)
	FLOAT	fltMinLimit;
		//default minimum graph limit (e.g. 0°C)

	char	szNameTemplate[MAX_PATH];
		//optional data source name template for multiGPU system, e.g. "GPU%d temperature" for "GPU1 temperature" name
		//the template is used to allow the host to extract GPU/CPU/etc index from the name
	char	szGroupTemplate[MAX_PATH];
		//optional data source group name template for multiGPU system, e.g. "GPU%d" for "GPU1" group name
		//the template is used to allow the host to extract GPU/CPU/etc index from the group name

} MONITORING_SOURCE_DESC, *LPMONITORING_SOURCE_DESC;
/////////////////////////////////////////////////////////////////////////////
#endif