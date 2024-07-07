#include <algorithm>
#include <unordered_map>
#include <format>

#include <dxgi.h>
#include <d3d11.h>
#include "../include/nvapi_interface.h"
#if defined __MINGW64__ || defined __MINGW32__
#include "../include/d3d12.h"
#else
#include <d3d12.h>
#endif
#include "../include/nvapi.h"
#include "nvapi.h"

#include "log.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        prepareLogging("nvapi-dummy.log");
        log("--------------");
        break;
    case DLL_PROCESS_DETACH:
        closeLogging();
        break;
    }
    return TRUE;
}

namespace nvd {
    extern "C" {
        NvAPI_Status __cdecl placeholder() {
            // return Ok();
            return Error(NVAPI_NO_IMPLEMENTATION);
        }

        static std::unordered_map<NvU32, void*> registry;

        __declspec(dllexport) void* __cdecl nvapi_QueryInterface(NvU32 id) {
            auto entry = registry.find(id);
            if (entry != registry.end())
                return entry->second;

            auto it = std::find_if(
                std::begin(nvapi_interface_table),
                std::end(nvapi_interface_table),
                [id](const auto& item) { return item.id == id; });

            if (it == std::end(nvapi_interface_table)) {
                log(std::format("NvAPI_QueryInterface (0x{:x}): Unknown interface ID", id));
                return registry.insert({ id, nullptr }).first->second;
            }

#define INSERT_AND_RETURN_WHEN_EQUALS(method) \
    if (std::string(it->func) == #method)     \
        return registry.insert({id, (void *)method}).first->second;
            INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_Initialize)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetInterfaceVersionString)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_EnumNvidiaDisplayHandle)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetLogicalGPUFromPhysicalGPU)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_EnumPhysicalGPUs)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_EnumLogicalGPUs)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetGPUIDfromPhysicalGPU)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetPhysicalGPUFromGPUID)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetPhysicalGPUsFromDisplay)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GetErrorMessage)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetLogicalGpuInfo)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetConnectedDisplayIds)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_CudaEnumComputeCapableGpus)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetArchInfo)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetPCIIdentifiers)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetFullName)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetGpuCoreCount)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetAllClockFrequencies)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_GPU_GetAdapterIdFromPhysicalGpu)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DISP_GetDisplayIdByDisplayName)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DISP_GetGDIPrimaryDisplayId)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_Disp_SetOutputMode)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_Disp_GetOutputMode)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_Mosaic_GetDisplayViewportsByResolution)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_SYS_GetDisplayDriverInfo)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_SYS_GetDriverAndBranchVersion)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_SYS_GetDisplayIdFromGpuAndOutputId)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_SYS_GetGpuAndOutputIdFromDisplayId)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_SetResourceHint)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_GetObjectHandleForResource)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_GetSleepStatus)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_GetLatency)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_SetSleepMode)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_SetLatencyMarker)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D_Sleep)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D11_IsNvShaderExtnOpCodeSupported)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D11_BeginUAVOverlap)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D11_EndUAVOverlap)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D11_SetDepthBoundsTest)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_GetRaytracingCaps)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_IsNvShaderExtnOpCodeSupported)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_SetNvShaderExtnSlotSpaceLocalThread)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_GetRaytracingAccelerationStructurePrebuildInfoEx)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_BuildRaytracingAccelerationStructureEx)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_NotifyOutOfBandCommandQueue)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_D3D12_SetAsyncFrameMarker)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DRS_CreateSession)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DRS_LoadSettings)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DRS_GetBaseProfile)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DRS_GetSetting)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_DRS_DestroySession)
                INSERT_AND_RETURN_WHEN_EQUALS(NvAPI_Unload)

#undef INSERT_AND_RETURN_WHEN_EQUALS

                log(std::format("{}: not implemented, placeholder given", it->func));
            return registry.insert({ id, (void*)placeholder }).first->second;
            // return registry.insert({ id, nullptr }).first->second;
        }
    }
}
