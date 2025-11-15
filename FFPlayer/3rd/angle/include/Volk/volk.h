/**
 * volk
 *
 * Copyright (C) 2018-2022, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at https://github.com/zeux/volk
 *
 * This library is distributed under the MIT License. See notice at the end of this file.
 */
/* clang-format off */
#ifndef VOLK_H_
#define VOLK_H_

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
#	error To use volk, you need to define VK_NO_PROTOTYPES before including vulkan.h
#endif

#if defined(EXPORT_VK_API)
#ifdef _WIN32
#define VK_EXPORT __declspec(dllexport)
#else
#define VK_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(_WIN32)
#define VK_EXPORT __declspec(dllimport)
#else
#define VK_EXPORT
#endif
#endif

/* VOLK_GENERATE_VERSION_DEFINE */
#define VOLK_HEADER_VERSION 224
/* VOLK_GENERATE_VERSION_DEFINE */

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
#       ifdef VOLK_VULKAN_H_PATH
#               include VOLK_VULKAN_H_PATH
#	elif defined(VK_USE_PLATFORM_WIN32_KHR)
#		include <vulkan/vk_platform.h>
#		include <vulkan/vulkan_core.h>

		/* When VK_USE_PLATFORM_WIN32_KHR is defined, instead of including vulkan.h directly, we include individual parts of the SDK
		 * This is necessary to avoid including <windows.h> which is very heavy - it takes 200ms to parse without WIN32_LEAN_AND_MEAN
		 * and 100ms to parse with it. vulkan_win32.h only needs a few symbols that are easy to redefine ourselves.
		 */
		typedef unsigned long DWORD;
		typedef const wchar_t* LPCWSTR;
		typedef void* HANDLE;
		typedef struct HINSTANCE__* HINSTANCE;
		typedef struct HWND__* HWND;
		typedef struct HMONITOR__* HMONITOR;
		typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;

#		include <vulkan/vulkan_win32.h>

#		ifdef VK_ENABLE_BETA_EXTENSIONS
#			include <vulkan/vulkan_beta.h>
#		endif
#	else
#		include <vulkan/vulkan.h>
#	endif
#endif

/* Disable several extensions on earlier SDKs because later SDKs introduce a backwards incompatible change to function signatures */
#if VK_HEADER_VERSION < 140
#	undef VK_NVX_image_view_handle
#endif
#if VK_HEADER_VERSION < 184
#	undef VK_HUAWEI_subpass_shading
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct VolkDeviceTable;

/**
 * Initialize library by loading Vulkan loader; call this function before creating the Vulkan instance.
 *
 * Returns VK_SUCCESS on success and VK_ERROR_INITIALIZATION_FAILED otherwise.
 */
VK_EXPORT VkResult volkInitialize(void);

/**
 * Initialize library by providing a custom handler to load global symbols.
 *
 * This function can be used instead of volkInitialize.
 * The handler function pointer will be asked to load global Vulkan symbols which require no instance
 * (such as vkCreateInstance, vkEnumerateInstance* and vkEnumerateInstanceVersion if available).
 */
VK_EXPORT void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler);

/**
 * Get Vulkan instance version supported by the Vulkan loader, or 0 if Vulkan isn't supported
 *
 * Returns 0 if volkInitialize wasn't called or failed.
 */
VK_EXPORT uint32_t volkGetInstanceVersion(void);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 */
VK_EXPORT void volkLoadInstance(VkInstance instance);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 * Skips loading device-based function pointers, requires usage of volkLoadDevice afterwards.
 */
VK_EXPORT void volkLoadInstanceOnly(VkInstance instance);

/**
 * Load global function pointers using application-created VkDevice; call this function after creating the Vulkan device.
 *
 * Note: this is not suitable for applications that want to use multiple VkDevice objects concurrently.
 */
VK_EXPORT void volkLoadDevice(VkDevice device);

/**
 * Return last VkInstance for which global function pointers have been loaded via volkLoadInstance(),
 * or VK_NULL_HANDLE if volkLoadInstance() has not been called.
 */
VK_EXPORT VkInstance volkGetLoadedInstance(void);

/**
 * Return last VkDevice for which global function pointers have been loaded via volkLoadDevice(),
 * or VK_NULL_HANDLE if volkLoadDevice() has not been called.
 */
VK_EXPORT VkDevice volkGetLoadedDevice(void);

/**
 * Load function pointers using application-created VkDevice into a table.
 * Application should use function pointers from that table instead of using global function pointers.
 */
VK_EXPORT void volkLoadDeviceTable(struct VolkDeviceTable* table, VkDevice device);

/**
 * Device-specific function pointer table
 */
struct VolkDeviceTable
{
	/* VOLK_GENERATE_DEVICE_TABLE */
#if defined(VK_VERSION_1_0)
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	PFN_vkAllocateMemory vkAllocateMemory;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	PFN_vkBindBufferMemory vkBindBufferMemory;
	PFN_vkBindImageMemory vkBindImageMemory;
	PFN_vkCmdBeginQuery vkCmdBeginQuery;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	PFN_vkCmdBindPipeline vkCmdBindPipeline;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	PFN_vkCmdBlitImage vkCmdBlitImage;
	PFN_vkCmdClearAttachments vkCmdClearAttachments;
	PFN_vkCmdClearColorImage vkCmdClearColorImage;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	PFN_vkCmdCopyImage vkCmdCopyImage;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	PFN_vkCmdDispatch vkCmdDispatch;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
	PFN_vkCmdDraw vkCmdDraw;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	PFN_vkCmdEndQuery vkCmdEndQuery;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	PFN_vkCmdFillBuffer vkCmdFillBuffer;
	PFN_vkCmdNextSubpass vkCmdNextSubpass;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	PFN_vkCmdPushConstants vkCmdPushConstants;
	PFN_vkCmdResetEvent vkCmdResetEvent;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	PFN_vkCmdResolveImage vkCmdResolveImage;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
	PFN_vkCmdSetEvent vkCmdSetEvent;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	PFN_vkCmdSetScissor vkCmdSetScissor;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
	PFN_vkCmdSetViewport vkCmdSetViewport;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
	PFN_vkCmdWaitEvents vkCmdWaitEvents;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
	PFN_vkCreateBuffer vkCreateBuffer;
	PFN_vkCreateBufferView vkCreateBufferView;
	PFN_vkCreateCommandPool vkCreateCommandPool;
	PFN_vkCreateComputePipelines vkCreateComputePipelines;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	PFN_vkCreateEvent vkCreateEvent;
	PFN_vkCreateFence vkCreateFence;
	PFN_vkCreateFramebuffer vkCreateFramebuffer;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	PFN_vkCreateImage vkCreateImage;
	PFN_vkCreateImageView vkCreateImageView;
	PFN_vkCreatePipelineCache vkCreatePipelineCache;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	PFN_vkCreateQueryPool vkCreateQueryPool;
	PFN_vkCreateRenderPass vkCreateRenderPass;
	PFN_vkCreateSampler vkCreateSampler;
	PFN_vkCreateSemaphore vkCreateSemaphore;
	PFN_vkCreateShaderModule vkCreateShaderModule;
	PFN_vkDestroyBuffer vkDestroyBuffer;
	PFN_vkDestroyBufferView vkDestroyBufferView;
	PFN_vkDestroyCommandPool vkDestroyCommandPool;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyEvent vkDestroyEvent;
	PFN_vkDestroyFence vkDestroyFence;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	PFN_vkDestroyImage vkDestroyImage;
	PFN_vkDestroyImageView vkDestroyImageView;
	PFN_vkDestroyPipeline vkDestroyPipeline;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	PFN_vkDestroyQueryPool vkDestroyQueryPool;
	PFN_vkDestroyRenderPass vkDestroyRenderPass;
	PFN_vkDestroySampler vkDestroySampler;
	PFN_vkDestroySemaphore vkDestroySemaphore;
	PFN_vkDestroyShaderModule vkDestroyShaderModule;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkEndCommandBuffer vkEndCommandBuffer;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
	PFN_vkFreeMemory vkFreeMemory;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkGetEventStatus vkGetEventStatus;
	PFN_vkGetFenceStatus vkGetFenceStatus;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
	PFN_vkMapMemory vkMapMemory;
	PFN_vkMergePipelineCaches vkMergePipelineCaches;
	PFN_vkQueueBindSparse vkQueueBindSparse;
	PFN_vkQueueSubmit vkQueueSubmit;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
	PFN_vkResetCommandBuffer vkResetCommandBuffer;
	PFN_vkResetCommandPool vkResetCommandPool;
	PFN_vkResetDescriptorPool vkResetDescriptorPool;
	PFN_vkResetEvent vkResetEvent;
	PFN_vkResetFences vkResetFences;
	PFN_vkSetEvent vkSetEvent;
	PFN_vkUnmapMemory vkUnmapMemory;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
	PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	PFN_vkBindBufferMemory2 vkBindBufferMemory2;
	PFN_vkBindImageMemory2 vkBindImageMemory2;
	PFN_vkCmdDispatchBase vkCmdDispatchBase;
	PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
	PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
	PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
	PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
	PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
	PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
	PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
	PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
	PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
	PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
	PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
	PFN_vkTrimCommandPool vkTrimCommandPool;
	PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
	PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
	PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
	PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
	PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
	PFN_vkCreateRenderPass2 vkCreateRenderPass2;
	PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
	PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
	PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
	PFN_vkResetQueryPool vkResetQueryPool;
	PFN_vkSignalSemaphore vkSignalSemaphore;
	PFN_vkWaitSemaphores vkWaitSemaphores;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	PFN_vkCmdBeginRendering vkCmdBeginRendering;
	PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
	PFN_vkCmdBlitImage2 vkCmdBlitImage2;
	PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
	PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
	PFN_vkCmdCopyImage2 vkCmdCopyImage2;
	PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
	PFN_vkCmdEndRendering vkCmdEndRendering;
	PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
	PFN_vkCmdResetEvent2 vkCmdResetEvent2;
	PFN_vkCmdResolveImage2 vkCmdResolveImage2;
	PFN_vkCmdSetCullMode vkCmdSetCullMode;
	PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
	PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
	PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
	PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
	PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
	PFN_vkCmdSetEvent2 vkCmdSetEvent2;
	PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
	PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
	PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
	PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
	PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
	PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
	PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
	PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
	PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
	PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
	PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
	PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
	PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
	PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
	PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
	PFN_vkGetPrivateData vkGetPrivateData;
	PFN_vkQueueSubmit2 vkQueueSubmit2;
	PFN_vkSetPrivateData vkSetPrivateData;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMD_buffer_marker)
	PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
	PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
	PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_buffer_device_address)
	PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
	PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_discard_rectangles)
	PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
	PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
	PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
	PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
	PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_extended_dynamic_state)
	PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
	PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
	PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
	PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
	PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
	PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
	PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
	PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
	PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
	PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
	PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
	PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#endif /* defined(VK_EXT_extended_dynamic_state) */
#if defined(VK_EXT_extended_dynamic_state2)
	PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
	PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
	PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
	PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
	PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#endif /* defined(VK_EXT_extended_dynamic_state2) */
#if defined(VK_EXT_external_memory_host)
	PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
	PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_query_reset)
	PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_compression_control)
	PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT;
#endif /* defined(VK_EXT_image_compression_control) */
#if defined(VK_EXT_image_drm_format_modifier)
	PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_metal_objects)
	PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
	PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_pageable_device_local_memory)
	PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
	PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
	PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
	PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
	PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT;
	PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT;
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_transform_feedback)
	PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
	PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
	PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
	PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
	PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
	PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
	PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
	PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
	PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_EXT_vertex_input_dynamic_state)
	PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif /* defined(VK_EXT_vertex_input_dynamic_state) */
#if defined(VK_FUCHSIA_buffer_collection)
	PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
	PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
	PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
	PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
	PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
	PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
	PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
	PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_invocation_mask)
	PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
	PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
	PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
	PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
	PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
	PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
	PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
	PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
	PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
	PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
	PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
	PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
	PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
	PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
	PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
	PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
	PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
	PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
	PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_copy_commands2)
	PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
	PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
	PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
	PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
	PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
	PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
	PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
	PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
	PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
	PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
	PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
	PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
	PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
	PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
	PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
	PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
	PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
	PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
	PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_external_fence_fd)
	PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
	PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
	PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
	PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
	PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
	PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
	PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
	PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
	PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_maintenance1)
	PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
	PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
	PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_performance_query)
	PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
	PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
	PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
	PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR;
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
	PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
	PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
	PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
	PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
	PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
	PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
	PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
	PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
	PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
	PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
	PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
	PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
	PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
	PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
	PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
	PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
	PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
	PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
	PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
	PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
	PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
	PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
	PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
	PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
	PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
	PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
	PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
	PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
	PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
	PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
	PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_external_memory_rdma)
	PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_mesh_shader)
	PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
	PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
	PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_ray_tracing)
	PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
	PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
	PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
	PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
	PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
	PFN_vkCompileDeferredNV vkCompileDeferredNV;
	PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
	PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
	PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
	PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
	PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
	PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive)
	PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
	PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
	PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
	PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM;
	PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM;
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
	PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
	PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_DEVICE_TABLE */
};

/* VOLK_GENERATE_PROTOTYPES_H */
#if defined(VK_VERSION_1_0)
VK_EXPORT extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
VK_EXPORT extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
VK_EXPORT extern PFN_vkAllocateMemory vkAllocateMemory;
VK_EXPORT extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
VK_EXPORT extern PFN_vkBindBufferMemory vkBindBufferMemory;
VK_EXPORT extern PFN_vkBindImageMemory vkBindImageMemory;
VK_EXPORT extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
VK_EXPORT extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
VK_EXPORT extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
VK_EXPORT extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
VK_EXPORT extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
VK_EXPORT extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
VK_EXPORT extern PFN_vkCmdBlitImage vkCmdBlitImage;
VK_EXPORT extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
VK_EXPORT extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
VK_EXPORT extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
VK_EXPORT extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
VK_EXPORT extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
VK_EXPORT extern PFN_vkCmdCopyImage vkCmdCopyImage;
VK_EXPORT extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
VK_EXPORT extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
VK_EXPORT extern PFN_vkCmdDispatch vkCmdDispatch;
VK_EXPORT extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
VK_EXPORT extern PFN_vkCmdDraw vkCmdDraw;
VK_EXPORT extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
VK_EXPORT extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
VK_EXPORT extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
VK_EXPORT extern PFN_vkCmdEndQuery vkCmdEndQuery;
VK_EXPORT extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
VK_EXPORT extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
VK_EXPORT extern PFN_vkCmdFillBuffer vkCmdFillBuffer;
VK_EXPORT extern PFN_vkCmdNextSubpass vkCmdNextSubpass;
VK_EXPORT extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
VK_EXPORT extern PFN_vkCmdPushConstants vkCmdPushConstants;
VK_EXPORT extern PFN_vkCmdResetEvent vkCmdResetEvent;
VK_EXPORT extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
VK_EXPORT extern PFN_vkCmdResolveImage vkCmdResolveImage;
VK_EXPORT extern PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
VK_EXPORT extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
VK_EXPORT extern PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
VK_EXPORT extern PFN_vkCmdSetEvent vkCmdSetEvent;
VK_EXPORT extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
VK_EXPORT extern PFN_vkCmdSetScissor vkCmdSetScissor;
VK_EXPORT extern PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
VK_EXPORT extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
VK_EXPORT extern PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
VK_EXPORT extern PFN_vkCmdSetViewport vkCmdSetViewport;
VK_EXPORT extern PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
VK_EXPORT extern PFN_vkCmdWaitEvents vkCmdWaitEvents;
VK_EXPORT extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
VK_EXPORT extern PFN_vkCreateBuffer vkCreateBuffer;
VK_EXPORT extern PFN_vkCreateBufferView vkCreateBufferView;
VK_EXPORT extern PFN_vkCreateCommandPool vkCreateCommandPool;
VK_EXPORT extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
VK_EXPORT extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
VK_EXPORT extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
VK_EXPORT extern PFN_vkCreateDevice vkCreateDevice;
VK_EXPORT extern PFN_vkCreateEvent vkCreateEvent;
VK_EXPORT extern PFN_vkCreateFence vkCreateFence;
VK_EXPORT extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
VK_EXPORT extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
VK_EXPORT extern PFN_vkCreateImage vkCreateImage;
VK_EXPORT extern PFN_vkCreateImageView vkCreateImageView;
VK_EXPORT extern PFN_vkCreateInstance vkCreateInstance;
VK_EXPORT extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
VK_EXPORT extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
VK_EXPORT extern PFN_vkCreateQueryPool vkCreateQueryPool;
VK_EXPORT extern PFN_vkCreateRenderPass vkCreateRenderPass;
VK_EXPORT extern PFN_vkCreateSampler vkCreateSampler;
VK_EXPORT extern PFN_vkCreateSemaphore vkCreateSemaphore;
VK_EXPORT extern PFN_vkCreateShaderModule vkCreateShaderModule;
VK_EXPORT extern PFN_vkDestroyBuffer vkDestroyBuffer;
VK_EXPORT extern PFN_vkDestroyBufferView vkDestroyBufferView;
VK_EXPORT extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
VK_EXPORT extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
VK_EXPORT extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
VK_EXPORT extern PFN_vkDestroyDevice vkDestroyDevice;
VK_EXPORT extern PFN_vkDestroyEvent vkDestroyEvent;
VK_EXPORT extern PFN_vkDestroyFence vkDestroyFence;
VK_EXPORT extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
VK_EXPORT extern PFN_vkDestroyImage vkDestroyImage;
VK_EXPORT extern PFN_vkDestroyImageView vkDestroyImageView;
VK_EXPORT extern PFN_vkDestroyInstance vkDestroyInstance;
VK_EXPORT extern PFN_vkDestroyPipeline vkDestroyPipeline;
VK_EXPORT extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
VK_EXPORT extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
VK_EXPORT extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
VK_EXPORT extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
VK_EXPORT extern PFN_vkDestroySampler vkDestroySampler;
VK_EXPORT extern PFN_vkDestroySemaphore vkDestroySemaphore;
VK_EXPORT extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
VK_EXPORT extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
VK_EXPORT extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
VK_EXPORT extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
VK_EXPORT extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
VK_EXPORT extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
VK_EXPORT extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
VK_EXPORT extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
VK_EXPORT extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
VK_EXPORT extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
VK_EXPORT extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
VK_EXPORT extern PFN_vkFreeMemory vkFreeMemory;
VK_EXPORT extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
VK_EXPORT extern PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
VK_EXPORT extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
VK_EXPORT extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
VK_EXPORT extern PFN_vkGetEventStatus vkGetEventStatus;
VK_EXPORT extern PFN_vkGetFenceStatus vkGetFenceStatus;
VK_EXPORT extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
VK_EXPORT extern PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
VK_EXPORT extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
VK_EXPORT extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
VK_EXPORT extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
VK_EXPORT extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
VK_EXPORT extern PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
VK_EXPORT extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
VK_EXPORT extern PFN_vkMapMemory vkMapMemory;
VK_EXPORT extern PFN_vkMergePipelineCaches vkMergePipelineCaches;
VK_EXPORT extern PFN_vkQueueBindSparse vkQueueBindSparse;
VK_EXPORT extern PFN_vkQueueSubmit vkQueueSubmit;
VK_EXPORT extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
VK_EXPORT extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
VK_EXPORT extern PFN_vkResetCommandPool vkResetCommandPool;
VK_EXPORT extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
VK_EXPORT extern PFN_vkResetEvent vkResetEvent;
VK_EXPORT extern PFN_vkResetFences vkResetFences;
VK_EXPORT extern PFN_vkSetEvent vkSetEvent;
VK_EXPORT extern PFN_vkUnmapMemory vkUnmapMemory;
VK_EXPORT extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
VK_EXPORT extern PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
VK_EXPORT extern PFN_vkBindBufferMemory2 vkBindBufferMemory2;
VK_EXPORT extern PFN_vkBindImageMemory2 vkBindImageMemory2;
VK_EXPORT extern PFN_vkCmdDispatchBase vkCmdDispatchBase;
VK_EXPORT extern PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
VK_EXPORT extern PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
VK_EXPORT extern PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
VK_EXPORT extern PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
VK_EXPORT extern PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
VK_EXPORT extern PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
VK_EXPORT extern PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
VK_EXPORT extern PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
VK_EXPORT extern PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
VK_EXPORT extern PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
VK_EXPORT extern PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
VK_EXPORT extern PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
VK_EXPORT extern PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
VK_EXPORT extern PFN_vkTrimCommandPool vkTrimCommandPool;
VK_EXPORT extern PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
VK_EXPORT extern PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
VK_EXPORT extern PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
VK_EXPORT extern PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
VK_EXPORT extern PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
VK_EXPORT extern PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
VK_EXPORT extern PFN_vkCreateRenderPass2 vkCreateRenderPass2;
VK_EXPORT extern PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
VK_EXPORT extern PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
VK_EXPORT extern PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
VK_EXPORT extern PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
VK_EXPORT extern PFN_vkResetQueryPool vkResetQueryPool;
VK_EXPORT extern PFN_vkSignalSemaphore vkSignalSemaphore;
VK_EXPORT extern PFN_vkWaitSemaphores vkWaitSemaphores;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
VK_EXPORT extern PFN_vkCmdBeginRendering vkCmdBeginRendering;
VK_EXPORT extern PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
VK_EXPORT extern PFN_vkCmdBlitImage2 vkCmdBlitImage2;
VK_EXPORT extern PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
VK_EXPORT extern PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
VK_EXPORT extern PFN_vkCmdCopyImage2 vkCmdCopyImage2;
VK_EXPORT extern PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
VK_EXPORT extern PFN_vkCmdEndRendering vkCmdEndRendering;
VK_EXPORT extern PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
VK_EXPORT extern PFN_vkCmdResetEvent2 vkCmdResetEvent2;
VK_EXPORT extern PFN_vkCmdResolveImage2 vkCmdResolveImage2;
VK_EXPORT extern PFN_vkCmdSetCullMode vkCmdSetCullMode;
VK_EXPORT extern PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
VK_EXPORT extern PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
VK_EXPORT extern PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
VK_EXPORT extern PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
VK_EXPORT extern PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
VK_EXPORT extern PFN_vkCmdSetEvent2 vkCmdSetEvent2;
VK_EXPORT extern PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
VK_EXPORT extern PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
VK_EXPORT extern PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
VK_EXPORT extern PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
VK_EXPORT extern PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
VK_EXPORT extern PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
VK_EXPORT extern PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
VK_EXPORT extern PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
VK_EXPORT extern PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
VK_EXPORT extern PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
VK_EXPORT extern PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
VK_EXPORT extern PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
VK_EXPORT extern PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
VK_EXPORT extern PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
VK_EXPORT extern PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
VK_EXPORT extern PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties;
VK_EXPORT extern PFN_vkGetPrivateData vkGetPrivateData;
VK_EXPORT extern PFN_vkQueueSubmit2 vkQueueSubmit2;
VK_EXPORT extern PFN_vkSetPrivateData vkSetPrivateData;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMD_buffer_marker)
VK_EXPORT extern PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
VK_EXPORT extern PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
VK_EXPORT extern PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
VK_EXPORT extern PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
VK_EXPORT extern PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
VK_EXPORT extern PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
VK_EXPORT extern PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_acquire_drm_display)
VK_EXPORT extern PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT;
VK_EXPORT extern PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT;
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
VK_EXPORT extern PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
VK_EXPORT extern PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_buffer_device_address)
VK_EXPORT extern PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
VK_EXPORT extern PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
VK_EXPORT extern PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
VK_EXPORT extern PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
VK_EXPORT extern PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
VK_EXPORT extern PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
VK_EXPORT extern PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
VK_EXPORT extern PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
VK_EXPORT extern PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
VK_EXPORT extern PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
VK_EXPORT extern PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
VK_EXPORT extern PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
VK_EXPORT extern PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
VK_EXPORT extern PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
VK_EXPORT extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
VK_EXPORT extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
VK_EXPORT extern PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
VK_EXPORT extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
VK_EXPORT extern PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
VK_EXPORT extern PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
VK_EXPORT extern PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
VK_EXPORT extern PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT;
VK_EXPORT extern PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_discard_rectangles)
VK_EXPORT extern PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
VK_EXPORT extern PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
VK_EXPORT extern PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
VK_EXPORT extern PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
VK_EXPORT extern PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_extended_dynamic_state)
VK_EXPORT extern PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
VK_EXPORT extern PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
VK_EXPORT extern PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
VK_EXPORT extern PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
VK_EXPORT extern PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
VK_EXPORT extern PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
VK_EXPORT extern PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
VK_EXPORT extern PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
VK_EXPORT extern PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
VK_EXPORT extern PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
VK_EXPORT extern PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
VK_EXPORT extern PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#endif /* defined(VK_EXT_extended_dynamic_state) */
#if defined(VK_EXT_extended_dynamic_state2)
VK_EXPORT extern PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
VK_EXPORT extern PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
VK_EXPORT extern PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
VK_EXPORT extern PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
VK_EXPORT extern PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#endif /* defined(VK_EXT_extended_dynamic_state2) */
#if defined(VK_EXT_external_memory_host)
VK_EXPORT extern PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
VK_EXPORT extern PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
VK_EXPORT extern PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
VK_EXPORT extern PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_headless_surface)
VK_EXPORT extern PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT;
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_host_query_reset)
VK_EXPORT extern PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_compression_control)
VK_EXPORT extern PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT;
#endif /* defined(VK_EXT_image_compression_control) */
#if defined(VK_EXT_image_drm_format_modifier)
VK_EXPORT extern PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
VK_EXPORT extern PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_metal_objects)
VK_EXPORT extern PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_metal_surface)
VK_EXPORT extern PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_multi_draw)
VK_EXPORT extern PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
VK_EXPORT extern PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_pageable_device_local_memory)
VK_EXPORT extern PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
VK_EXPORT extern PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
VK_EXPORT extern PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
VK_EXPORT extern PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
VK_EXPORT extern PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
VK_EXPORT extern PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
VK_EXPORT extern PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
VK_EXPORT extern PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
VK_EXPORT extern PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT;
VK_EXPORT extern PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT;
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_tooling_info)
VK_EXPORT extern PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT;
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_EXT_transform_feedback)
VK_EXPORT extern PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
VK_EXPORT extern PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
VK_EXPORT extern PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
VK_EXPORT extern PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
VK_EXPORT extern PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
VK_EXPORT extern PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
VK_EXPORT extern PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
VK_EXPORT extern PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
VK_EXPORT extern PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
VK_EXPORT extern PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_EXT_vertex_input_dynamic_state)
VK_EXPORT extern PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif /* defined(VK_EXT_vertex_input_dynamic_state) */
#if defined(VK_FUCHSIA_buffer_collection)
VK_EXPORT extern PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
VK_EXPORT extern PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
VK_EXPORT extern PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
VK_EXPORT extern PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
VK_EXPORT extern PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
VK_EXPORT extern PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
VK_EXPORT extern PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
VK_EXPORT extern PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
VK_EXPORT extern PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_FUCHSIA_imagepipe_surface)
VK_EXPORT extern PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
VK_EXPORT extern PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_GOOGLE_display_timing)
VK_EXPORT extern PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
VK_EXPORT extern PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_invocation_mask)
VK_EXPORT extern PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
VK_EXPORT extern PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
VK_EXPORT extern PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
VK_EXPORT extern PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
VK_EXPORT extern PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
VK_EXPORT extern PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
VK_EXPORT extern PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
VK_EXPORT extern PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
VK_EXPORT extern PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
VK_EXPORT extern PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
VK_EXPORT extern PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
VK_EXPORT extern PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
VK_EXPORT extern PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
VK_EXPORT extern PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
VK_EXPORT extern PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
VK_EXPORT extern PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
VK_EXPORT extern PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
VK_EXPORT extern PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
VK_EXPORT extern PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
VK_EXPORT extern PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
VK_EXPORT extern PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
VK_EXPORT extern PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
VK_EXPORT extern PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
VK_EXPORT extern PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
VK_EXPORT extern PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
VK_EXPORT extern PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
VK_EXPORT extern PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
VK_EXPORT extern PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_android_surface)
VK_EXPORT extern PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
VK_EXPORT extern PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
VK_EXPORT extern PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
VK_EXPORT extern PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
VK_EXPORT extern PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
VK_EXPORT extern PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_copy_commands2)
VK_EXPORT extern PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
VK_EXPORT extern PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
VK_EXPORT extern PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
VK_EXPORT extern PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
VK_EXPORT extern PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
VK_EXPORT extern PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
VK_EXPORT extern PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
VK_EXPORT extern PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
VK_EXPORT extern PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
VK_EXPORT extern PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
VK_EXPORT extern PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
VK_EXPORT extern PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
VK_EXPORT extern PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
VK_EXPORT extern PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
VK_EXPORT extern PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
VK_EXPORT extern PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
VK_EXPORT extern PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
VK_EXPORT extern PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
VK_EXPORT extern PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
VK_EXPORT extern PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
VK_EXPORT extern PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
VK_EXPORT extern PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
VK_EXPORT extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
VK_EXPORT extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
VK_EXPORT extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
VK_EXPORT extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
VK_EXPORT extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
VK_EXPORT extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
VK_EXPORT extern PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
VK_EXPORT extern PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
VK_EXPORT extern PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
VK_EXPORT extern PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_external_fence_capabilities)
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
VK_EXPORT extern PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
VK_EXPORT extern PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
VK_EXPORT extern PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
VK_EXPORT extern PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
VK_EXPORT extern PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
VK_EXPORT extern PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
VK_EXPORT extern PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
VK_EXPORT extern PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
VK_EXPORT extern PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
VK_EXPORT extern PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
VK_EXPORT extern PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
VK_EXPORT extern PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
VK_EXPORT extern PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
VK_EXPORT extern PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
VK_EXPORT extern PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
VK_EXPORT extern PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
VK_EXPORT extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
VK_EXPORT extern PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
VK_EXPORT extern PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_maintenance1)
VK_EXPORT extern PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
VK_EXPORT extern PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
VK_EXPORT extern PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
VK_EXPORT extern PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
VK_EXPORT extern PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_performance_query)
VK_EXPORT extern PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
VK_EXPORT extern PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
VK_EXPORT extern PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
VK_EXPORT extern PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
VK_EXPORT extern PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
VK_EXPORT extern PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
VK_EXPORT extern PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
VK_EXPORT extern PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
VK_EXPORT extern PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR;
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
VK_EXPORT extern PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
VK_EXPORT extern PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
VK_EXPORT extern PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
VK_EXPORT extern PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
VK_EXPORT extern PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
VK_EXPORT extern PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
VK_EXPORT extern PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
VK_EXPORT extern PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
VK_EXPORT extern PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
VK_EXPORT extern PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
VK_EXPORT extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
VK_EXPORT extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
VK_EXPORT extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
VK_EXPORT extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
VK_EXPORT extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
VK_EXPORT extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
VK_EXPORT extern PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
VK_EXPORT extern PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
VK_EXPORT extern PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
VK_EXPORT extern PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
VK_EXPORT extern PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
VK_EXPORT extern PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
VK_EXPORT extern PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
VK_EXPORT extern PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
VK_EXPORT extern PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
VK_EXPORT extern PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
VK_EXPORT extern PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
VK_EXPORT extern PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
VK_EXPORT extern PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
VK_EXPORT extern PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
VK_EXPORT extern PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
VK_EXPORT extern PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
VK_EXPORT extern PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
VK_EXPORT extern PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
VK_EXPORT extern PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
VK_EXPORT extern PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
VK_EXPORT extern PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
VK_EXPORT extern PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
VK_EXPORT extern PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
VK_EXPORT extern PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
VK_EXPORT extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
VK_EXPORT extern PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
VK_EXPORT extern PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
VK_EXPORT extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
VK_EXPORT extern PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
VK_EXPORT extern PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
VK_EXPORT extern PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_binary_import)
VK_EXPORT extern PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
VK_EXPORT extern PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
VK_EXPORT extern PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
VK_EXPORT extern PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
VK_EXPORT extern PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
VK_EXPORT extern PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
VK_EXPORT extern PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_acquire_winrt_display)
VK_EXPORT extern PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV;
VK_EXPORT extern PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV;
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_clip_space_w_scaling)
VK_EXPORT extern PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cooperative_matrix)
VK_EXPORT extern PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_coverage_reduction_mode)
VK_EXPORT extern PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_device_diagnostic_checkpoints)
VK_EXPORT extern PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
VK_EXPORT extern PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
VK_EXPORT extern PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
VK_EXPORT extern PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
VK_EXPORT extern PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
VK_EXPORT extern PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
VK_EXPORT extern PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
VK_EXPORT extern PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_external_memory_capabilities)
VK_EXPORT extern PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_rdma)
VK_EXPORT extern PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
VK_EXPORT extern PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
VK_EXPORT extern PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_mesh_shader)
VK_EXPORT extern PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
VK_EXPORT extern PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
VK_EXPORT extern PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_ray_tracing)
VK_EXPORT extern PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
VK_EXPORT extern PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
VK_EXPORT extern PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
VK_EXPORT extern PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
VK_EXPORT extern PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
VK_EXPORT extern PFN_vkCompileDeferredNV vkCompileDeferredNV;
VK_EXPORT extern PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
VK_EXPORT extern PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
VK_EXPORT extern PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
VK_EXPORT extern PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
VK_EXPORT extern PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
VK_EXPORT extern PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive)
VK_EXPORT extern PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
VK_EXPORT extern PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
VK_EXPORT extern PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
VK_EXPORT extern PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
VK_EXPORT extern PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM;
VK_EXPORT extern PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM;
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_screen_surface)
VK_EXPORT extern PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX;
VK_EXPORT extern PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX;
#endif /* defined(VK_QNX_screen_surface) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
VK_EXPORT extern PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
VK_EXPORT extern PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
VK_EXPORT extern PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
VK_EXPORT extern PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
VK_EXPORT extern PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
VK_EXPORT extern PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
VK_EXPORT extern PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
VK_EXPORT extern PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
/* VOLK_GENERATE_PROTOTYPES_H */

#ifdef __cplusplus
}
#endif

#endif

#ifdef VOLK_IMPLEMENTATION
#undef VOLK_IMPLEMENTATION
// Prevent tools like dependency checkers that don't evaluate
// macros from detecting a cyclic dependency.
#define VOLK_SOURCE "volk.c"
#include VOLK_SOURCE
#endif

/**
 * Copyright (c) 2018-2022 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
/* clang-format on */
