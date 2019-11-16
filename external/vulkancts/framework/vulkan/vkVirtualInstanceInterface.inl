/* WARNING: This is auto-generated file. Do not modify, since changes will
 * be lost! Modify the generating script instead.
 */
virtual void		destroyInstance													(VkInstance instance, const VkAllocationCallbacks* pAllocator) const = 0;
virtual VkResult	enumeratePhysicalDevices										(VkInstance instance, deUint32* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const = 0;
virtual void		getPhysicalDeviceFeatures										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures* pFeatures) const = 0;
virtual void		getPhysicalDeviceFormatProperties								(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties* pFormatProperties) const = 0;
virtual VkResult	getPhysicalDeviceImageFormatProperties							(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) const = 0;
virtual void		getPhysicalDeviceProperties										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties) const = 0;
virtual void		getPhysicalDeviceQueueFamilyProperties							(VkPhysicalDevice physicalDevice, deUint32* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) const = 0;
virtual void		getPhysicalDeviceMemoryProperties								(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties* pMemoryProperties) const = 0;
virtual VkResult	createDevice													(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) const = 0;
virtual VkResult	enumerateDeviceExtensionProperties								(VkPhysicalDevice physicalDevice, const char* pLayerName, deUint32* pPropertyCount, VkExtensionProperties* pProperties) const = 0;
virtual VkResult	enumerateDeviceLayerProperties									(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkLayerProperties* pProperties) const = 0;
virtual void		getPhysicalDeviceSparseImageFormatProperties					(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, deUint32* pPropertyCount, VkSparseImageFormatProperties* pProperties) const = 0;
virtual VkResult	enumeratePhysicalDeviceGroups									(VkInstance instance, deUint32* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const = 0;
virtual void		getPhysicalDeviceFeatures2										(VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures) const = 0;
virtual void		getPhysicalDeviceProperties2									(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties) const = 0;
virtual void		getPhysicalDeviceFormatProperties2								(VkPhysicalDevice physicalDevice, VkFormat format, VkFormatProperties2* pFormatProperties) const = 0;
virtual VkResult	getPhysicalDeviceImageFormatProperties2							(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const = 0;
virtual void		getPhysicalDeviceQueueFamilyProperties2							(VkPhysicalDevice physicalDevice, deUint32* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const = 0;
virtual void		getPhysicalDeviceMemoryProperties2								(VkPhysicalDevice physicalDevice, VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const = 0;
virtual void		getPhysicalDeviceSparseImageFormatProperties2					(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, deUint32* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const = 0;
virtual void		getPhysicalDeviceExternalBufferProperties						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const = 0;
virtual void		getPhysicalDeviceExternalFenceProperties						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const = 0;
virtual void		getPhysicalDeviceExternalSemaphoreProperties					(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const = 0;
virtual void		destroySurfaceKHR												(VkInstance instance, VkSurfaceKHR surface, const VkAllocationCallbacks* pAllocator) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceSupportKHR								(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceCapabilitiesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceFormatsKHR								(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const = 0;
virtual VkResult	getPhysicalDeviceSurfacePresentModesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pPresentModeCount, VkPresentModeKHR* pPresentModes) const = 0;
virtual VkResult	getPhysicalDevicePresentRectanglesKHR							(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, deUint32* pRectCount, VkRect2D* pRects) const = 0;
virtual VkResult	getPhysicalDeviceDisplayPropertiesKHR							(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPropertiesKHR* pProperties) const = 0;
virtual VkResult	getPhysicalDeviceDisplayPlanePropertiesKHR						(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) const = 0;
virtual VkResult	getDisplayPlaneSupportedDisplaysKHR								(VkPhysicalDevice physicalDevice, deUint32 planeIndex, deUint32* pDisplayCount, VkDisplayKHR* pDisplays) const = 0;
virtual VkResult	getDisplayModePropertiesKHR										(VkPhysicalDevice physicalDevice, VkDisplayKHR display, deUint32* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) const = 0;
virtual VkResult	createDisplayModeKHR											(VkPhysicalDevice physicalDevice, VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDisplayModeKHR* pMode) const = 0;
virtual VkResult	getDisplayPlaneCapabilitiesKHR									(VkPhysicalDevice physicalDevice, VkDisplayModeKHR mode, deUint32 planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) const = 0;
virtual VkResult	createDisplayPlaneSurfaceKHR									(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	enumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR	(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, deUint32* pCounterCount, VkPerformanceCounterKHR* pCounters, VkPerformanceCounterDescriptionKHR* pCounterDescriptions) const = 0;
virtual void		getPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR			(VkPhysicalDevice physicalDevice, const VkQueryPoolPerformanceCreateInfoKHR* pPerformanceQueryCreateInfo, deUint32* pNumPasses) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceCapabilities2KHR						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceFormats2KHR								(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, deUint32* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const = 0;
virtual VkResult	getPhysicalDeviceDisplayProperties2KHR							(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayProperties2KHR* pProperties) const = 0;
virtual VkResult	getPhysicalDeviceDisplayPlaneProperties2KHR						(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) const = 0;
virtual VkResult	getDisplayModeProperties2KHR									(VkPhysicalDevice physicalDevice, VkDisplayKHR display, deUint32* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) const = 0;
virtual VkResult	getDisplayPlaneCapabilities2KHR									(VkPhysicalDevice physicalDevice, const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) const = 0;
virtual VkResult	createDebugReportCallbackEXT									(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) const = 0;
virtual void		destroyDebugReportCallbackEXT									(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) const = 0;
virtual void		debugReportMessageEXT											(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, deUint64 object, deUintptr location, deInt32 messageCode, const char* pLayerPrefix, const char* pMessage) const = 0;
virtual VkResult	getPhysicalDeviceExternalImageFormatPropertiesNV				(VkPhysicalDevice physicalDevice, VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) const = 0;
virtual void		getPhysicalDeviceGeneratedCommandsPropertiesNVX					(VkPhysicalDevice physicalDevice, VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits) const = 0;
virtual VkResult	releaseDisplayEXT												(VkPhysicalDevice physicalDevice, VkDisplayKHR display) const = 0;
virtual VkResult	getPhysicalDeviceSurfaceCapabilities2EXT						(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) const = 0;
virtual VkResult	createDebugUtilsMessengerEXT									(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) const = 0;
virtual void		destroyDebugUtilsMessengerEXT									(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) const = 0;
virtual void		submitDebugUtilsMessageEXT										(VkInstance instance, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const = 0;
virtual void		getPhysicalDeviceMultisamplePropertiesEXT						(VkPhysicalDevice physicalDevice, VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) const = 0;
virtual VkResult	getPhysicalDeviceCalibrateableTimeDomainsEXT					(VkPhysicalDevice physicalDevice, deUint32* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const = 0;
virtual VkResult	getPhysicalDeviceCooperativeMatrixPropertiesNV					(VkPhysicalDevice physicalDevice, deUint32* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) const = 0;
virtual VkResult	getPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV	(VkPhysicalDevice physicalDevice, deUint32* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) const = 0;
virtual VkResult	createHeadlessSurfaceEXT										(VkInstance instance, const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createAndroidSurfaceKHR											(VkInstance instance, const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createImagePipeSurfaceFUCHSIA									(VkInstance instance, const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createStreamDescriptorSurfaceGGP								(VkInstance instance, const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createIOSSurfaceMVK												(VkInstance instance, const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createMacOSSurfaceMVK											(VkInstance instance, const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createMetalSurfaceEXT											(VkInstance instance, const VkMetalSurfaceCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createViSurfaceNN												(VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkResult	createWaylandSurfaceKHR											(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkBool32	getPhysicalDeviceWaylandPresentationSupportKHR					(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::WaylandDisplayPtr display) const = 0;
virtual VkResult	createWin32SurfaceKHR											(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkBool32	getPhysicalDeviceWin32PresentationSupportKHR					(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex) const = 0;
virtual VkResult	getPhysicalDeviceSurfacePresentModes2EXT						(VkPhysicalDevice physicalDevice, const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, deUint32* pPresentModeCount, VkPresentModeKHR* pPresentModes) const = 0;
virtual VkResult	createXcbSurfaceKHR												(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkBool32	getPhysicalDeviceXcbPresentationSupportKHR						(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::XcbConnectionPtr connection, pt::XcbVisualid visual_id) const = 0;
virtual VkResult	createXlibSurfaceKHR											(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) const = 0;
virtual VkBool32	getPhysicalDeviceXlibPresentationSupportKHR						(VkPhysicalDevice physicalDevice, deUint32 queueFamilyIndex, pt::XlibDisplayPtr dpy, pt::XlibVisualID visualID) const = 0;
virtual VkResult	acquireXlibDisplayEXT											(VkPhysicalDevice physicalDevice, pt::XlibDisplayPtr dpy, VkDisplayKHR display) const = 0;
virtual VkResult	getRandROutputDisplayEXT										(VkPhysicalDevice physicalDevice, pt::XlibDisplayPtr dpy, pt::RROutput rrOutput, VkDisplayKHR* pDisplay) const = 0;
