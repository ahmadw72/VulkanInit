#include "VulkanInit.h"
#include "Vertex.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include "glm\gtc\matrix_transform.hpp"
#define ENABLE_DEBUG_LAYERS 1

#ifdef ENABLE_DEBUG_LAYERS
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT obj, uint64_t uiObj, size_t uiLoc, int32_t iMsgCode, const char* pszLayerPrefix, const char* pszMsg, void* pUsrData)
{
	printf("DebugCallBack --- msg = %s\n", pszMsg);

	return VK_FALSE;
}

#endif

/**
	 * @brief 
	 * Lets summarize 
	 * 1. the first step in vulkan application is to initilaze the vulkan. we need to grasp few concepts about vulkan
	 * surface:   The area in the window where the vulkan will render the 3D graphics
	 * image/images: the memory that will contain the bits that will be displayed on the surface, i.e., the photos 
	 * ques:  every work that is submited to gpu is qued up. four types (family ) of ques
	 * 		present:   			used for graphics rendering
	 * 		compute:   			computational work
	 * 		transfer:  			copying buffers from one location to another
	 * 		memory management: 	sparse memory correction 
	 * 
	 *  step 1:  query the system for all the available device 'vkEnumeratePhysicalDevices'
	 * 	step 2:  query each device for the available ques. Remember, its 'family of ques'. one many many family can have many ques.   
	 * 				and then the attributes of each quefamily will be read into a property sturct, called quefamilStructure   'vkGetPhysicalDeviceQueueFamilyProperties'
	 * 			step 2.1: for each combination of device + surface  and que, check if it can can be used to render images 'vkGetPhysicalDeviceSurfaceSupportKHR'
	 * 			step 2.2: for each combination of device and surface enumerate the avalable formats 'vkGetPhysicalDeviceSurfaceFormatsKHR'. Formats is the sequence of data for pixel. 
	 * 			step 2.3: for each combine of device and surface enumerate the surface capabilities vkGetPhysicalDeviceSurfaceCapabilitiesKHR
	 * 
	 * step 3: create the logical device 
	 * 			3.1 fill in a VkDeviceQueCreateInfo struct
	 * 			3.2 fill in a VkDeviceCreateInfo info; it will require the struct filled in previous step
	 * 			3.3 call VKCreateDevice
	 * step 4. create swap chain and  command buffers for the the logical device
	 * 			4.1 fill in a VkSwapchainCreateInfoKHR and then call vkCreateSwapchainKHR to create the swap chain
	 * 			4.2 to create the command buffer :
	 * 				4.2.1 create a command pool. Fill in a VkCommandPoolCreateInfo struct and then call vkCreateCommandPool
	 * 				4.2.2 create the command buffer. Fill in VkCommandBufferAllocateInfo struct, it will need the command pool that was 
	 * 						created in the previous step + the number of images in the swapchain to resize the commandbuffer Vecotr.
	 * 				4.2.3 call vkAllocateCommandBuffers  
	 * 
	 */



/**
 * @brief 
 * 	return the current surface format
 * @return VkFormat 
 */
VkFormat CVulkanInit::GetCurrnetSurfaceFormat()
{
	assert( m_iDeviceIndex != -1 && m_iQIndex != -1);
	return m_Devices.m_VkSurfaceFormats[m_iDeviceIndex][m_iQIndex].format;
}
/**
 * @brief 
 * 
 * @param memPropFlagBits 
 * @param uiType 
 * @return int 
 */
int CVulkanInit::FindMemoryIndexByFlagandType(VkMemoryPropertyFlagBits memPropFlagBits, uint32_t uiMemTypeBits)
{
	VkPhysicalDeviceMemoryProperties memProps = {};
	vkGetPhysicalDeviceMemoryProperties(GetSelectedPhysicalDevice(), &memProps);

	for(UINT uiCount=0; uiCount<memProps.memoryTypeCount; uiCount++)
	{
		VkMemoryType vkMemType = memProps.memoryTypes[uiCount];
		VkMemoryPropertyFlags vkPropFlags = vkMemType.propertyFlags;

		if (uiMemTypeBits && (1<<uiCount))
		{
			if (vkPropFlags && memPropFlagBits)
			{
				printf("FindMemoryIndexByFlagandType(VkMemoryPropertyFlagBits memPropFlagBits, uint32_t uiMemTypeBits) -> found \n");
				return uiCount;
			}
		}

	}
	assert(false);  // shouldnt come here //
	return -1; 

}


void CVulkanInit::AvailableLayers()
{
	uint32_t uiCount = 0;

	vkEnumerateInstanceLayerProperties(&uiCount, NULL);

	//()
	std::vector<VkLayerProperties> VkLayerProps;
	VkLayerProps.resize(uiCount);
	vkEnumerateInstanceLayerProperties(&uiCount, &VkLayerProps[0]);

	for (auto prop : VkLayerProps)
		printf(" AvailableLayers():  %s \n",  prop.layerName);


//	vkEnumerateDeviceLayerProperties(m_Devices.m_VkPhysicalDevices[m_iDeviceIndex], &uiCount, nullptr);
	VkLayerProps.resize(uiCount);

	for (auto prop : VkLayerProps)
		printf(" AvailableLayers physical device properties():  %s \n",  prop.layerName);




}


/**
 * @brief Construct a new CVulkanInit::CVulkanInit object
 * 
 */
CVulkanInit::CVulkanInit()
{
	m_iWindowHeight = 0;
	m_iWindowWidth =0;
	
	m_iDeviceIndex = -1;
	m_iQIndex = -1;

	m_VkSurfaceKHR = VK_NULL_HANDLE;
	m_VkRenderPass = VK_NULL_HANDLE;
	
	m_VKFragmentShaderModule = VK_NULL_HANDLE;
	m_VKVertexShaderModule = VK_NULL_HANDLE;

	m_VkDevice = VK_NULL_HANDLE;
	m_VkCommandPool = VK_NULL_HANDLE;
	// available extension for the instance//
	/*UINT uiInstPropCount=0;
	VkResult res =  vkEnumerateInstanceExtensionProperties(NULL, &uiInstPropCount, NULL);

	if (res != VK_SUCCESS)
	{
		printf("failed vkEnumerateInstanceExtensionProperties\n");
		return;
	}

	m_InstExtProps.resize(uiInstPropCount);

	res = vkEnumerateInstanceExtensionProperties(NULL, &uiInstPropCount, &m_InstExtProps[0]);

	if (res != VK_SUCCESS)
	{
		printf("failed vkEnumerateInstanceExtensionProperties\n");
		return;
	}

	// print the instance extension properties //

	for(UINT uiCount=0; uiCount<uiInstPropCount; uiCount++)
	{
		const VkExtensionProperties& rExtProp = m_InstExtProps[uiCount];
		printf("Instance Extension Properties [%d] name = %s  version = %d\n", uiCount, rExtProp.extensionName, rExtProp.specVersion);

	}

	UINT uiInstLayerPropsCount = 0;

	if ( vkEnumerateInstanceLayerProperties(&uiInstLayerPropsCount, NULL) != VK_SUCCESS )
	{
		printf(" failed vkEnumerateInstanceLayerProperties(&uiInstLayerProps, nullptr) \n" );
		return;
	}

	std::vector< VkLayerProperties > vLayerProps;
	
	vLayerProps.resize(uiInstLayerPropsCount);

	if ( vkEnumerateInstanceLayerProperties(&uiInstLayerPropsCount, &vLayerProps[0] ) != VK_SUCCESS )
	{
		printf(" failed vkEnumerateInstanceLayerProperties(&uiInstLayerProps, &vLayerProps[0]) \n" );
		return;
	}

	for ( auto layerProps : vLayerProps)
	{
		printf("Instance Layer Properperties = %s \n", layerProps.layerName);
	}






	VkApplicationInfo vkAppInfo = {};

	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "BlueThunder";
	vkAppInfo.engineVersion = 1;
	vkAppInfo.apiVersion = VK_API_VERSION_1_2;
/*
	uint32_t uiExtCount=1;
	const char*	pszInstExt[] ={
	VK_KHR_SURFACE_EXTENSION_NAME,
	#ifdef ENABLE_DEBUG_LAYERS			// we have disabled debug layer, as the computer does'nt support debug layer //
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	#endif
	#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	#endif
		
	};
*/
/*	m_vExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	#ifdef ENABLE_DEBUG_LAYERS			// we have disabled debug layer, as the computer does'nt support debug layer //
		m_vExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		m_vLayers.push_back("VK_LAYER_KHRONOS_validation");
	#endif
	#ifdef WIN32
		m_vExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#endif
*/
/*
	#ifdef ENABLE_DEBUG_LAYERS			
		uiExtCount++;
	#endif
	#ifdef _WIN32
		uiExtCount++; 
	#endif

	uint32_t uiLayersCount = 0;	
	
	

	#ifdef ENABLE_DEBUG_LAYERS
		const char* pszInstLayers[] ={
			"VK_LAYER_LURANG_standard_validation"

		};
		uiLayersCount++;
	#endif

*/
/*	VkInstanceCreateInfo vkInstCreateInfo ={};

	vkInstCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstCreateInfo.pApplicationInfo = &vkAppInfo;
	//vkInstCreateInfo.enabledLayerCount = uiLayersCount;
	
	vkInstCreateInfo.enabledLayerCount = m_vLayers.size();

	//vkInstCreateInfo.ppEnabledExtensionNames = pszInstExt;
	// vkInstCreateInfo.enabledExtensionCount = uiExtCount;

	vkInstCreateInfo.ppEnabledExtensionNames = &m_vExtensions[0];
	vkInstCreateInfo.enabledExtensionCount = m_vExtensions.size();

	#ifdef ENABLE_DEBUG_LAYERS
		//vkInstCreateInfo.ppEnabledLayerNames = pszInstLayers;
		vkInstCreateInfo.ppEnabledLayerNames = &m_vLayers[0];
		//vkInstCreateInfo.enabledLayerCount = uiLayersCount;
	#endif
	
	

	res =  vkCreateInstance(&vkInstCreateInfo, NULL, &m_VkInstance);

	if (res != VK_SUCCESS)
	{
		printf("vkCreateInstace failed\n");
	}

	if (res == VK_ERROR_LAYER_NOT_PRESENT && m_VkInstance != NULL)
	{
		("Debug layer not present \n");
		return;
	}

	#ifdef ENABLE_DEBUG_LAYERS
	
	 VkDebugReportCallbackCreateInfoEXT  callBackInfo = {};
	 callBackInfo.flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	 callBackInfo.pfnCallback = &DebugCallBack;
	 callBackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;

	 VkDebugReportCallbackEXT callBackExt;
	 PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
	 CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_VkInstance, "vkCreateDebugReportCallbackEXT");
	 
	// res = vkCreateDebugReportCallbackEXT(m_VkInstance, &callBackInfo, NULL,&callBackExt);
	 res = CreateDebugReportCallback(m_VkInstance, &callBackInfo, NULL,&callBackExt);

	 if(res != VK_SUCCESS)
	 {
		printf("vkCreateDebugReportCallbackExt failed \n");
	 }
	
	
	#endif
*/

}
/**
 * @brief 
 *   creats vulkan instance
 * 	1. enumerates extensions properties
 * 	2. enumerates instance layer properties
 *  3. create the instance with requires extensions and layers 
 * 
 * @return int -1 -> error
 * 				otherwise  
 */

int  CVulkanInit::CreateInstance_01()
{
	UINT uiInstPropCount=0;
	VkResult res =  vkEnumerateInstanceExtensionProperties(NULL, &uiInstPropCount, NULL);

	if (res != VK_SUCCESS)
	{
		printf("failed vkEnumerateInstanceExtensionProperties\n");
		return -1;
	}

	m_InstExtProps.resize(uiInstPropCount);

	res = vkEnumerateInstanceExtensionProperties(NULL, &uiInstPropCount, &m_InstExtProps[0]);

	if (res != VK_SUCCESS)
	{
		printf("failed vkEnumerateInstanceExtensionProperties\n");
		return -1;
	}

	// print the instance extension properties //

	for(UINT uiCount=0; uiCount<uiInstPropCount; uiCount++)
	{
		const VkExtensionProperties& rExtProp = m_InstExtProps[uiCount];
		printf("Instance Extension Properties [%d] name = %s  version = %d\n", uiCount, rExtProp.extensionName, rExtProp.specVersion);

	}

	UINT uiInstLayerPropsCount = 0;

	if ( vkEnumerateInstanceLayerProperties(&uiInstLayerPropsCount, NULL) != VK_SUCCESS )
	{
		printf(" failed vkEnumerateInstanceLayerProperties(&uiInstLayerProps, nullptr) \n" );
		return -1;
	}

	std::vector< VkLayerProperties > vLayerProps;
	
	vLayerProps.resize(uiInstLayerPropsCount);

	if ( vkEnumerateInstanceLayerProperties(&uiInstLayerPropsCount, &vLayerProps[0] ) != VK_SUCCESS )
	{
		printf(" failed vkEnumerateInstanceLayerProperties(&uiInstLayerProps, &vLayerProps[0]) \n" );
		return -1;
	}

	for ( auto layerProps : vLayerProps)
	{
		printf("Instance Layer Properperties = %s \n", layerProps.layerName);
	}






	VkApplicationInfo vkAppInfo = {};

	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "BlueThunder";
	vkAppInfo.engineVersion = 1;
	vkAppInfo.apiVersion = VK_API_VERSION_1_2;
/*
	uint32_t uiExtCount=1;
	const char*	pszInstExt[] ={
	VK_KHR_SURFACE_EXTENSION_NAME,
	#ifdef ENABLE_DEBUG_LAYERS			// we have disabled debug layer, as the computer does'nt support debug layer //
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	#endif
	#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	#endif
		
	};
*/
	m_vExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	#ifdef ENABLE_DEBUG_LAYERS			// we have disabled debug layer, as the computer does'nt support debug layer //
		m_vExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		m_vLayers.push_back("VK_LAYER_KHRONOS_validation");
	#endif
	#ifdef WIN32
		m_vExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
	#endif

/*
	#ifdef ENABLE_DEBUG_LAYERS			
		uiExtCount++;
	#endif
	#ifdef _WIN32
		uiExtCount++; 
	#endif

	uint32_t uiLayersCount = 0;	
	
	

	#ifdef ENABLE_DEBUG_LAYERS
		const char* pszInstLayers[] ={
			"VK_LAYER_LURANG_standard_validation"

		};
		uiLayersCount++;
	#endif

*/
	VkInstanceCreateInfo vkInstCreateInfo ={};

	vkInstCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstCreateInfo.pApplicationInfo = &vkAppInfo;
	//vkInstCreateInfo.enabledLayerCount = uiLayersCount;
	
	vkInstCreateInfo.enabledLayerCount = m_vLayers.size();

	//vkInstCreateInfo.ppEnabledExtensionNames = pszInstExt;
	// vkInstCreateInfo.enabledExtensionCount = uiExtCount;

	vkInstCreateInfo.ppEnabledExtensionNames = &m_vExtensions[0];
	vkInstCreateInfo.enabledExtensionCount = m_vExtensions.size();

	#ifdef ENABLE_DEBUG_LAYERS
		//vkInstCreateInfo.ppEnabledLayerNames = pszInstLayers;
		vkInstCreateInfo.ppEnabledLayerNames = &m_vLayers[0];
		//vkInstCreateInfo.enabledLayerCount = uiLayersCount;
	#endif
	
	

	res =  vkCreateInstance(&vkInstCreateInfo, NULL, &m_VkInstance);

	if (res != VK_SUCCESS)
	{
		printf("vkCreateInstace failed\n");
		return -1;
	}

	if (res == VK_ERROR_LAYER_NOT_PRESENT && m_VkInstance != NULL)
	{
		("Debug layer not present \n");
		return -1;
	}

	#ifdef ENABLE_DEBUG_LAYERS
	
	 VkDebugReportCallbackCreateInfoEXT  callBackInfo = {};
	 callBackInfo.flags = VK_DEBUG_REPORT_DEBUG_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	 callBackInfo.pfnCallback = &DebugCallBack;
	 callBackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;

	 VkDebugReportCallbackEXT callBackExt;
	 PFN_vkCreateDebugReportCallbackEXT CreateDebugReportCallback = VK_NULL_HANDLE;
	 CreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_VkInstance, "vkCreateDebugReportCallbackEXT");
	 
	// res = vkCreateDebugReportCallbackEXT(m_VkInstance, &callBackInfo, NULL,&callBackExt);
	 res = CreateDebugReportCallback(m_VkInstance, &callBackInfo, NULL,&callBackExt);

	 if(res != VK_SUCCESS)
	 {
		printf("vkCreateDebugReportCallbackExt failed \n");
		return -1;
	 }
	
	
	#endif
	return 0;

}

bool CVulkanInit::Init(HWND hwnd, HINSTANCE hinst, int iWndHeight, int iWndWidth)
{
//	AvailableLayers();
	CreateInstance_01();

	CreateSurfaceWin32_02(hwnd, hinst, iWndHeight, iWndWidth);

	GetPhysicalDevice_03();

    CreateLogicalDevice_04();

	CreateSwapChain_05();
	CreateCommandBuffers_06();

	

    CreateRenderPass_07();

	CreateFrameBuffer_08();

    // create shader //
    
	CreateVertexBuffer_09();
	CreateUniformBuffer_10();
   
	m_Texture.CreateFromBMP(m_Devices.m_VkPhysicalDevices[m_iDeviceIndex] ,m_VkDevice,m_VKPresentQue, m_VkCommandPool, "Texture01.bmp");
	//m_Texture.CreateFromBMP(m_Devices.m_VkPhysicalDevices[m_iDeviceIndex] ,m_VkDevice,m_VKPresentQue, m_VkCommandPool, "Texture01.jpg");
	
	CreateDescriptorSets_12();
 	
   // RecordCommandBuffer();
	
	CreatePipeline_13();
	CreateSynchObjects_14();

	return true;

}
/**
 * @brief creates win32surface
 * 
 * @param hWnd  handle to window
 * @param hInstance  instance
 * @param iHeight   window Height
 * @param iWidth 	window width
 * @return VkBool32 
 */

VkBool32 CVulkanInit::CreateSurfaceWin32_02(HWND hWnd, HINSTANCE hInstance, int iHeight, int iWidth)
{
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};

	surfaceCreateInfo.hwnd = hWnd;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = hInstance;


	VkBool32 res = vkCreateWin32SurfaceKHR(m_VkInstance, &surfaceCreateInfo, NULL, &m_VkSurfaceKHR);

	if (res != VK_SUCCESS )
	{
		printf("vkCreateWin32SurfaceKHR failed\n");
		assert(false); 
	}

	m_iWindowHeight = iHeight;
	m_iWindowWidth = iWidth;

	return res;

}
/*
	1. enumerate the physical devices (the real GPu)
	2. for each physical device 
		i. get all the VkPhysicalDeviceProperties
		ii. get all the VkQueFamilyProperties
		iii. Check which queue supports present (capable of drawing on screen)
		iv. get all the VkSurfaceFormate supported by the device

*/
int CVulkanInit::GetPhysicalDevice_03()
{

    UINT uiNumDevices =0;
	VkResult res = vkEnumeratePhysicalDevices(m_VkInstance, &uiNumDevices, NULL); 
	
	if (res != VK_SUCCESS)
	{
		printf("vkEnumeratePhysicalDevices failed\n");
		return -1;

	}
	m_Devices.m_VkPhysicalDevices.resize(uiNumDevices);
	m_Devices.m_VkPhysicalDeviceProperties.resize(uiNumDevices);
	m_Devices.m_VkQueuefamilyProperties.resize(uiNumDevices);
	m_Devices. m_bQueueSupportsPresent.resize(uiNumDevices);
	m_Devices.m_VkSurfaceFormats.resize(uiNumDevices);
	// now get the physical devices, second call to vkEnumeratePhysicalDevices //
	
	res = vkEnumeratePhysicalDevices(m_VkInstance, &uiNumDevices, &m_Devices.m_VkPhysicalDevices[0]);   // &m_Devices.m_PhysicalDevices[0]  is the address of the array that will store the VkPhysicalDevices structs //

	if (res != VK_SUCCESS)
	{
		printf("vkEnumeratePhysicalDevices(m_VkInstance, &uiNumDevices, &m_Devices.m_VkPhysicalDevices[0]) failed\n");
		return -1;

	}
	
	
	//  next step, get the properties of each device //

	for (UINT uiCount=0; uiCount<uiNumDevices; uiCount++)
	{
		vkGetPhysicalDeviceProperties(m_Devices.m_VkPhysicalDevices[uiCount], &m_Devices.m_VkPhysicalDeviceProperties[uiCount]);
		/*
			print the properties of each device here if needed
		*/
		printf("Device name: %s\t", m_Devices.m_VkPhysicalDeviceProperties[uiCount].deviceName);
		uint32_t uiVer = m_Devices.m_VkPhysicalDeviceProperties[uiCount].apiVersion;
		printf("API Major version %d\tAPI Minor version %d\tPatch%d\n", VK_API_VERSION_MAJOR(uiVer), VK_API_VERSION_MINOR(uiVer), VK_API_VERSION_PATCH(uiVer));
		
	}
	// step 3. get the queues 
	// every work is submited into the device through queueus //
	// we have four types of queues //
	// 1. rendering
	// 2. compute
	// 3. image copy
	// 4. sparse memory correction  

    for (UINT uiCount=0; uiCount< uiNumDevices; uiCount++)
	{
		UINT uiNumQueProps=0;
		//VkResult res = VK_FALSE;

		vkGetPhysicalDeviceQueueFamilyProperties(m_Devices.m_VkPhysicalDevices[uiCount], &uiNumQueProps, NULL); // get the number of ques for this device //

		printf("device[%d] no. of  queProps = %d \n", uiCount, uiNumQueProps );

		m_Devices.m_VkQueuefamilyProperties[uiCount].resize(uiNumQueProps);
		vkGetPhysicalDeviceQueueFamilyProperties(m_Devices.m_VkPhysicalDevices[uiCount], &uiNumQueProps, &m_Devices.m_VkQueuefamilyProperties[uiCount][0]); // get the number of ques for this device //

		// we now check if a particular combination of surface+que can be used for 3D presentation //

		m_Devices.m_bQueueSupportsPresent[uiCount].resize(uiNumQueProps);

		for(UINT uiQFamilyIndex = 0; uiQFamilyIndex < uiNumQueProps; uiQFamilyIndex++)
		{	
			//print queProp details here // 
			// https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#VkQueueFlagBits //
			const VkQueueFamilyProperties& rqueProp =  m_Devices.m_VkQueuefamilyProperties[uiCount][uiQFamilyIndex];
			UINT uiNumQues = rqueProp.queueCount;
			printf("\tdevice[%d] queProp[%d] ->  No. of ques =%d\n ", uiCount, uiQFamilyIndex, uiNumQues);

			for (UINT uiQueIndex = 0; uiQueIndex < uiNumQues; uiQueIndex++)
			{
				UINT uiqueFlags = rqueProp.queueFlags;

				if( uiqueFlags & VK_QUEUE_GRAPHICS_BIT == VK_QUEUE_GRAPHICS_BIT)
					printf("\t\tVK_QUEUE_GRAPHICS_BIT present\n");
				if(uiqueFlags & VK_QUEUE_COMPUTE_BIT == VK_QUEUE_COMPUTE_BIT)
					printf("\t\tVK_QUEUE_COMPUTE_BIT present\n");
				if(uiqueFlags & VK_QUEUE_TRANSFER_BIT == VK_QUEUE_TRANSFER_BIT)
					printf("\t\tVK_QUEUE_TRANSFER_BIT present\n");
				if(uiqueFlags & VK_QUEUE_SPARSE_BINDING_BIT == VK_QUEUE_SPARSE_BINDING_BIT)
					printf("\t\tVK_QUEUE_SPARSE_BINDING_BIT present\n");
				if(uiqueFlags & VK_QUEUE_PROTECTED_BIT == VK_QUEUE_PROTECTED_BIT)
					printf("\t\tVK_QUEUE_PROTECTED_BIT present\n");
				/*
				if(uiqueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR == VK_QUEUE_VIDEO_DECODE_BIT_KHR)
					printf("\t\tVK_QUEUE_VIDEO_DECODE_BIT_KHR presnt");
				if(uiqueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR == VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
					printf("\t\tVK_QUEUE_VIDEO_ENCODE_BIT_KHR presnt");
				if(uiqueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV  == VK_QUEUE_OPTICAL_FLOW_BIT_NV )
					printf("\t\tVK_QUEUE_OPTICAL_FLOW_BIT_NV  presnt");
				*/
			}
				
				// check if this que + surface can support presentation ? //
				res = vkGetPhysicalDeviceSurfaceSupportKHR(m_Devices.m_VkPhysicalDevices[uiCount], uiQFamilyIndex, m_VkSurfaceKHR,  &m_Devices.m_bQueueSupportsPresent[uiCount][uiQFamilyIndex]);
				
				if (res != VK_SUCCESS)
				{
					printf("vkGetPhysicalDeviceSurfaceSupportKHR(m_Devices.m_VkPhysicalDevices[uiCount], uiQFamilyIndex, m_VkSurfaceKHR,  &m_Devices.m_bQueueSupportsPresent[uiCount][uiQFamilyIndex]) failed\n");
					return -1;
				}
		}

		// the next and the last part of this function is prepare the vulkan surface related stuff//

		// step 1.   get the surface formats, surface formats are vertex formats ... position+colorinof+texture coords etc //

		uint32_t uiSurfaceFormatsCount = 0;

		res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &uiSurfaceFormatsCount, NULL);
		
		if (res != VK_SUCCESS)
		{
			printf("vkGetPhysicalDeviceSurfaceFormatsKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &uiSurfaceFormatsCount, NULL) failed\n");
			return -1;
		}
		
		
		m_Devices.m_VkSurfaceFormats[uiCount].resize(uiSurfaceFormatsCount);

		res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &uiSurfaceFormatsCount, &m_Devices.m_VkSurfaceFormats[uiCount][0]);

		if (res != VK_SUCCESS)
		{
			printf("vkGetPhysicalDeviceSurfaceFormatsKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &uiSurfaceFormatsCount, &m_Devices.m_VkSurfaceFormats[uiCount][0]) failed\n");
			return -1;
		}
		
		
		for (UINT uiSurfaceFormatIndex=0; uiSurfaceFormatIndex < uiSurfaceFormatsCount; uiSurfaceFormatIndex++)
		{
			/*
			print the available surface formats //
			*/
			printf("Device # %d surfaceformat #%d colorspare = %d format = %d \n", uiCount, uiSurfaceFormatIndex,  m_Devices.m_VkSurfaceFormats[uiCount][uiSurfaceFormatIndex].colorSpace, m_Devices.m_VkSurfaceFormats[uiCount][uiSurfaceFormatIndex].format);

		}

		// step 2. get surface capabilities. surface capabilities is the swap chain related info, mainly //

		m_Devices.m_VkSurfaceCapabilities.resize(uiNumDevices);

		res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &m_Devices.m_VkSurfaceCapabilities[uiCount]);

		if (res != VK_SUCCESS)
		{
			printf("vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Devices.m_VkPhysicalDevices[uiCount], m_VkSurfaceKHR, &m_Devices.m_VkSurfaceCapabilities[uiCount]) failed\n");
			return -1;
		}

		for (UINT uiCapCount=0; uiCapCount< uiCount; uiCount++)
		{
			VkSurfaceCapabilitiesKHR& rCaps = m_Devices.m_VkSurfaceCapabilities[uiCapCount];

			printf("device[%d] SurfaceCaps[%d] minImageCount=%d\n", uiCount, uiCapCount, rCaps.minImageCount);
			printf("device[%d] SurfaceCaps[%d] maxImageCount=%d\n", uiCount, uiCapCount, rCaps.maxImageCount); 


		}
		// end of  getting physical device info //
		// next step is to create the logical device based upon the information  we have gathered in this function.
		// lets go through Create LogicalDevice.
	}

		return 0;
}

int CVulkanInit::CreateLogicalDevice_04()
{
	// step1. find the suitable queue and device //

	UINT uiNumDevices = m_Devices.m_VkPhysicalDevices.size();

	for(UINT uiDeviceIndex=0; uiDeviceIndex < uiNumDevices; uiDeviceIndex++ )
	{
		UINT uiNumQues = m_Devices.m_VkQueuefamilyProperties[uiDeviceIndex].size();

		for (UINT uiQIndex=0; uiQIndex < uiNumQues; uiQIndex++ )
		{
			VkQueueFamilyProperties& rQProps = m_Devices.m_VkQueuefamilyProperties[uiDeviceIndex][uiQIndex];
			VkQueueFlags qFlags = rQProps.queueFlags;
			VkBool32 b =   m_Devices.m_bQueueSupportsPresent[uiDeviceIndex][uiQIndex];

			if (qFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (!m_Devices.m_bQueueSupportsPresent[uiDeviceIndex][uiQIndex] )
						{
							printf("q with graphic bit but no present support");
							continue;
						}
				m_iDeviceIndex = uiDeviceIndex;
				m_iQIndex = uiQIndex;
				break;
			}

		}

	}

	//	assert(m_iDeviceIndex != -1 || m_iQIndex !=- 1)
	// now create the logical device //
	// logical device is a software layer abstraction on  physcal device. we need to provide the right q info that our logical device will
	// have //
	// for rendering we need to create: 1. a swap chain 2. and command buffer 'attached' with this logical device //

	// create q, we need to fill in VkDeviceQueueCreteInfo struct; this will 
	// be refrenced by the devicecreateinfo struct //
	VkDeviceQueueCreateInfo qInfo = {};
	qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qInfo.queueCount = 1;
	qInfo.queueFamilyIndex = m_iQIndex;
	float fQPriorities = 1.0f;
	qInfo.pQueuePriorities = &fQPriorities;

	// we now fill in VkDeviceCreateInfo struct,

	VkDeviceCreateInfo devInfo = {};

	devInfo.pQueueCreateInfos = &qInfo;
	devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	devInfo.queueCreateInfoCount = 1;
	devInfo.pQueueCreateInfos = &qInfo;

	const char* pszDevExt[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	devInfo.enabledExtensionCount = 1;
	devInfo.ppEnabledExtensionNames = pszDevExt;

	// create the device now //
	VkResult res = vkCreateDevice(m_Devices.m_VkPhysicalDevices[m_iDeviceIndex], &devInfo, NULL, &m_VkDevice);

	if (res != VK_SUCCESS)
	{
		printf("vkCreaeDevice failed\n");
		return -1;

	}

	return 1;
}
	// now creae the swap chain //
int CVulkanInit::CreateSwapChain_05()
{

	VkSurfaceCapabilitiesKHR& rSurfaceCap = m_Devices.m_VkSurfaceCapabilities[m_iDeviceIndex];

	if (rSurfaceCap.currentExtent.width < 0)  // width will be -1 for a bad surface //
 	{
		printf(" error, rSurfaceCap.currentExtent.width \n");
		return -1;
	}

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = m_VkSurfaceKHR;
	swapChainCreateInfo.minImageCount = m_Devices.m_VkSurfaceCapabilities[m_iDeviceIndex].minImageCount; // 2;
	swapChainCreateInfo.imageFormat = m_Devices.m_VkSurfaceFormats[m_iDeviceIndex][m_iQIndex].format;
	swapChainCreateInfo.imageColorSpace = m_Devices.m_VkSurfaceFormats[m_iDeviceIndex][m_iQIndex].colorSpace;
	swapChainCreateInfo.imageExtent = m_Devices.m_VkSurfaceCapabilities[m_iDeviceIndex].currentExtent;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // https://www.reddit.com/r/vulkan/comments/9txqqb/what_is_presentation_mode/  very precise definition of different presentation modes //
	swapChainCreateInfo.clipped = true;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;


	VkResult res = vkCreateSwapchainKHR(m_VkDevice, &swapChainCreateInfo, NULL, &m_VkSwapChain);

	if (res != VK_SUCCESS)
	{
		printf("vkCreateSwapChainKHR failed\n");
		return -1;

	}
	
	UINT uiNumImages =0;

	res = vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChain, &uiNumImages, NULL);
	if (res != VK_SUCCESS)
	{
		printf("vkGetSwapChainImagesKHR failed\n");
		return -1;

	}
	
	m_VkSwapChainImages.resize(uiNumImages);
//	m_VkFrameBuffers.resize(uiNumImages);
	res  = vkGetSwapchainImagesKHR(m_VkDevice, m_VkSwapChain, &uiNumImages, &m_VkSwapChainImages[0]);

	if (res != VK_SUCCESS)
	{
		printf("vkGetSwapChainImagesKHR (2) failed\n");
		return -1;

	}
 return 1;
}
	/**
	 * @brief Creates command buffer
	 * 
	 * @return int 
	 */
int CVulkanInit::CreateCommandBuffers_06()
{
	//step 1: allocate a command pool 
	VkCommandPoolCreateInfo cmdPoolCreateInfo = {};
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = m_iQIndex;  // que that has (1) presentation support and (2) can work with given surface, physical device for presentation

	VkResult res = vkCreateCommandPool(m_VkDevice, &cmdPoolCreateInfo, NULL, &m_VkCommandPool);

	if (res != VK_SUCCESS)
	{
		printf("vkCreateCommandPool failed\n");
		return -1;

	}
	// step 2. create command buffer //

	VkCommandBufferAllocateInfo cmdBufferAllocInfo ={};
	cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocInfo.commandPool = m_VkCommandPool;
	cmdBufferAllocInfo.commandBufferCount = m_VkSwapChainImages.size();
	cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_VkCommandBuffers.resize(m_VkSwapChainImages.size()); //

	res = vkAllocateCommandBuffers(m_VkDevice, &cmdBufferAllocInfo,&m_VkCommandBuffers[0]);

	if (res != VK_SUCCESS)
	{
		printf("vkAllocateCommandBuffers failed\n");
		return -1;

	}
	m_VkImageViews.resize(m_VkSwapChainImages.size());

	vkGetDeviceQueue(m_VkDevice,m_iQIndex,0 ,&m_VKPresentQue);
	
	return 1;
}

/**
 * @brief retunrs current physical device
 * 
 */

VkPhysicalDevice CVulkanInit::GetSelectedPhysicalDevice()
{
	assert(m_iDeviceIndex > -1) ;   // device not initialed//
	return m_Devices.m_VkPhysicalDevices[m_iDeviceIndex];
}
/**
 * @brief 
 * 
 */
 void CVulkanInit::CreateDescriptorSets_12()
 {
	CreateDescriptorPool();
	CreateDescriptorSetLayouts();
	CreateDescriptorSets();

 }

/**
 * @brief 
 * 
 */
void CVulkanInit::CreateDescriptorPool()
{
	/*VkDescriptorPoolSize descriptorPoolSize ={};
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSize.descriptorCount = 1;*/

	
	std::array< VkDescriptorPoolSize, 2> descriptorPoolSizes;
	descriptorPoolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorPoolSizes[0].descriptorCount = 1;

	descriptorPoolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorPoolSizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo  desriptorPoolCreateInfo ={};

	desriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	desriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
	desriptorPoolCreateInfo.poolSizeCount  = descriptorPoolSizes.size();
	desriptorPoolCreateInfo.maxSets = 1;

	if( vkCreateDescriptorPool(m_VkDevice, &desriptorPoolCreateInfo, nullptr, &m_VkDescriptorPool) != VK_SUCCESS )
	{

		printf("vkCreateDescriptorPool(m_VkDevice, &desriptorPoolCreateInfo, nullptr, &m_VkDescriptorPool) .. failed\n");
		assert(false);
	}

}
/**
 * @brief 
 * 
 */

void CVulkanInit::CreateDescriptorSetLayouts()
{
	/*VkDescriptorSetLayoutBinding descSetLayoutBinding = {};
	descSetLayoutBinding.descriptorCount = 1;
	descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descSetLayoutBinding.binding = 0;
	descSetLayoutBinding.pImmutableSamplers = nullptr;
	descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;*/

	std::array<VkDescriptorSetLayoutBinding, 2> descSetLayoutBindings = {};
	descSetLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descSetLayoutBindings[0].binding = 0;
	descSetLayoutBindings[0].descriptorCount = 1;
	descSetLayoutBindings[0].pImmutableSamplers = nullptr;
	descSetLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	descSetLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descSetLayoutBindings[1].binding = 1;
	descSetLayoutBindings[1].descriptorCount = 1;
	descSetLayoutBindings[1].pImmutableSamplers = nullptr;
	descSetLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	//descSetLayoutBindings[1].

	VkDescriptorSetLayoutCreateInfo descSetLayoutCreateInfo = {};
	descSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutCreateInfo.pBindings = descSetLayoutBindings.data();
	descSetLayoutCreateInfo.bindingCount = descSetLayoutBindings.size();
	descSetLayoutCreateInfo.pNext = nullptr;


	if (vkCreateDescriptorSetLayout(m_VkDevice, &descSetLayoutCreateInfo, nullptr, &m_VkDescriptorSetLayout) != VK_SUCCESS )
	{

		printf("vkCreateDescriptorSetLayout(m_VkDevice, &descSetLayoutCreateInfo, nullptr, &m_VkDescriptorSetLayout) failed..\n");
		assert(false);
	}
	
}
/**
 * @brief 
 * 
 */

void CVulkanInit::CreateUniformBuffer_10()
{
	//glm::mat4 v = 
	glm::mat4 m = glm::mat4(1.0f);

	glm::mat4 v = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );

	glm::mat4 p = glm::perspective(glm::radians(45.0f),(float) m_iWindowWidth/m_iWindowHeight, 0.1f, 10.0f);

	VkPhysicalDevice physicalDevice = m_Devices.m_VkPhysicalDevices[m_iDeviceIndex];
	m_UniformBuffer.Create(physicalDevice, m_VkDevice, m,v,p);


}
/**
 * @brief 
 * 
 */
void CVulkanInit::CreateDescriptorSets()
{

	VkDescriptorSetAllocateInfo descAllocInfo = {};

	descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descAllocInfo.descriptorPool = m_VkDescriptorPool;
	descAllocInfo.descriptorSetCount = 1;
	descAllocInfo.pSetLayouts = &m_VkDescriptorSetLayout;

	size_t descSetSize(1);  // only one set at the moment //
	m_vVkDescriptorSets.resize(descSetSize);


	if ( vkAllocateDescriptorSets(m_VkDevice, &descAllocInfo, m_vVkDescriptorSets.data() ) != VK_SUCCESS )
	{
		printf("vkAllocateDescriptorSets(m_VkDevice, &descAllocInfo, m_vVkDescriptorSets.data() ) faile..\n");
		assert(false);

	}


	VkDescriptorBufferInfo  descBuffInfo={};

	descBuffInfo.buffer = m_UniformBuffer.GetVkBuffer();
	descBuffInfo.offset = 0;
	descBuffInfo.range = sizeof(stMVP);


	VkDescriptorImageInfo	descImageInfo = {};

	descImageInfo.imageView = m_Texture.GetImageView();
	descImageInfo.sampler = m_Texture.GetSampler();
	descImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

     std::array<	VkWriteDescriptorSet, 2>  writeDescSets = {};
	writeDescSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSets[0].descriptorCount = 1;
	writeDescSets[0].pBufferInfo = &descBuffInfo;
	writeDescSets[0].dstSet = m_vVkDescriptorSets[0];
	writeDescSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescSets[0].dstBinding = 0;
	writeDescSets[0].dstArrayElement = 0;

	writeDescSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDescSets[1].descriptorCount = 1;
	writeDescSets[1].pImageInfo = &descImageInfo;
	writeDescSets[1].dstSet = m_vVkDescriptorSets[0];
	writeDescSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	writeDescSets[1].dstBinding = 1;
	writeDescSets[1].dstArrayElement = 0;



	vkUpdateDescriptorSets(m_VkDevice, writeDescSets.size(), writeDescSets.data(), 0, nullptr);

}
/**
 * @brief 
 * 
 * somethig that actually burns in the render loop is framebuffer.
 * 
 * 
 * @return int 
 */

int CVulkanInit::CreateRenderPass_07()
{
	VkAttachmentDescription attachmentDesc = {};
	attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDesc.format = GetCurrnetSurfaceFormat();
	attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// creation of subpass //
		//create attachmetn refernece

		VkAttachmentReference attachmentRef = {};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subPassDesc = {};
	subPassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPassDesc.pColorAttachments = &attachmentRef;
	subPassDesc.colorAttachmentCount = 1;

	VkRenderPassCreateInfo renderPassCreateInfo ={};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDesc;

	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subPassDesc;

	VkResult res =  vkCreateRenderPass(m_VkDevice, &renderPassCreateInfo, NULL, &m_VkRenderPass);

	if (res != VK_SUCCESS )
	{
		printf("vkCreateRenderPass(m_VkDevice, &renderPassCreateInfo, NULL, &m_VkRenderPass) failed\n");
		return -1;
	}

}
/**
 * @brief 
 * 
 * @return int 
 

int CVulkanInit::CreatePipeLine()
{
	VkGraphicsPipelineCreateInfo  pipeLineInfo = {};
	pipeLineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	//pipeLineInfo.




}*/


/**
 * @brief creaetes shader
 * 
 */
int CVulkanInit::CreateShader(const char* pszShaderFileName, VkShaderModule& rVkShaderModule)
{
	//ifstream

	int iRes = 0;

	std::ifstream  fstrShader(pszShaderFileName, std::ifstream::binary | std::ifstream::ate);
	//fstrShader.open(pszShaderFileName, std::ios::binary | std::ios::ate);

	if (!fstrShader)
	{
		printf("failed to open" );
		return -1;

	}

	fstrShader.seekg(0, fstrShader.end);

	int iSize = fstrShader.tellg();
	std::vector<char> vBuff(iSize);

	fstrShader.seekg(0, fstrShader.beg);
	fstrShader.read(vBuff.data(), iSize);

	if (!fstrShader)
	{
		printf("failed to opne shared file %s\n", pszShaderFileName);
		return -1;
	}

	VkShaderModuleCreateInfo shaderCreateInfo;

	shaderCreateInfo.codeSize = iSize;
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo.pCode = (const uint32_t*) vBuff.data();


	VkResult vRes = vkCreateShaderModule(m_VkDevice, &shaderCreateInfo, NULL, &rVkShaderModule);

	if (vRes != VK_SUCCESS)
	{
		printf("vkCreateShaderModule(m_VkDevice, NULL, NULL, &rVkShaderModule) failed\n");
		return -1;
	}

	
	return iRes;

}


int CVulkanInit::CreateFrameBuffer_08()
{
	UINT uiNumImageViews = m_VkImageViews.size();

	m_VkFrameBuffers.resize(uiNumImageViews);

	//VkResult res = VK_FALSE;

	for(UINT uiCount=0; uiCount<uiNumImageViews; uiCount++)
	{
		VkImageViewCreateInfo  imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageViewCreateInfo.image = m_VkSwapChainImages[uiCount];
		imageViewCreateInfo.format = GetCurrnetSurfaceFormat();
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

		imageViewCreateInfo.components.r = imageViewCreateInfo.components.g = imageViewCreateInfo.components.b = imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		VkResult res = vkCreateImageView(m_VkDevice,&imageViewCreateInfo, NULL, &m_VkImageViews[uiCount]);

		if ( res != VK_SUCCESS)
		{
			printf("vkCreateImageView(m_VkDevice,&imageViewCreateInfo, NULL, &m_VkImageViews[uiCount]) failed \n");
			assert(false);

		} 

		// now create framebuffer ..//

		VkFramebufferCreateInfo  framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		
		assert(m_VkRenderPass != VK_NULL_HANDLE);  // create render pass //
		
		framebufferCreateInfo.renderPass = m_VkRenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &m_VkImageViews[uiCount];


		framebufferCreateInfo.width =  m_iWindowWidth;
		framebufferCreateInfo.height = m_iWindowHeight;

		framebufferCreateInfo.layers = 1;

		res = vkCreateFramebuffer(m_VkDevice, &framebufferCreateInfo, NULL, &m_VkFrameBuffers[uiCount]);

		if (res != VK_SUCCESS)
		{
			printf("vkCreateFramebuffer(m_VkDevice, &framebufferCreateInfo, NULL, &m_VkFrameBuffers[uiCount]) failed");
			assert(false);
			return -1;
		}
	}

}
/**
 * @brief creates rendering pipeline
 * 
 * @return int 
 */

int CVulkanInit::CreatePipeline_13()
{
	
	
	// shader info //

	//CreateShader("shaders\\trgvs.spv", m_VKVertexShaderModule);// vertex shader //
	CreateShader("shaders\\vertexShader1.spv", m_VKVertexShaderModule);// vertex shader //

	//CreateShader("shaders\\trgfs.spv", m_VKFragmentShaderModule); // fragment shader //
	CreateShader("shaders\\fragShader1.spv", m_VKFragmentShaderModule); // fragment shader //
	assert(m_VKFragmentShaderModule != VK_NULL_HANDLE && m_VKVertexShaderModule != VK_NULL_HANDLE ); // shaders not initialzed called createshader //

	VkPipelineShaderStageCreateInfo shaderCreateInfo[2] = {};

	shaderCreateInfo[0].module = m_VKVertexShaderModule;
	shaderCreateInfo[0].pName = "main";
	shaderCreateInfo[0].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;


	shaderCreateInfo[1].module = m_VKFragmentShaderModule;
	shaderCreateInfo[1].pName = "main";
	shaderCreateInfo[1].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;


	// vertex input info//

	//m_VertexBuffer.
	//CVertex vertex;
	auto attribDesc = m_VertexBuffer.GetVertexInputAttributeDescription();
	auto inputBidingDesc = m_VertexBuffer.GetVertexInputBindingDescription();
	
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1 ; 
	vertexInputCreateInfo.pVertexBindingDescriptions = &inputBidingDesc;

	vertexInputCreateInfo.vertexAttributeDescriptionCount = (uint32_t)attribDesc.size();;
	vertexInputCreateInfo.pVertexAttributeDescriptions = attribDesc.data();
	

	//
	VkViewport vp={};
	vp.x = 0;
	vp.y = 0;

	vp.minDepth = 0.0f;
	vp.maxDepth = 1.0f;

	vp.height = m_iWindowHeight;
	vp.width = m_iWindowWidth;

	VkPipelineViewportStateCreateInfo viewPortCreateInfo = {};
	viewPortCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewPortCreateInfo.viewportCount = 1;
	viewPortCreateInfo.pViewports = &vp;
	
	// rasterization //


	VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo={};
	rasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationCreateInfo.cullMode = VK_CULL_MODE_NONE;
	rasterizationCreateInfo.lineWidth = 1.0f;
	rasterizationCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;


	// sampling //

	VkPipelineMultisampleStateCreateInfo multisampleCreateInfo	 = {};
	multisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	
	// blend//

	VkPipelineColorBlendAttachmentState blendAttachmentState = {};
	blendAttachmentState.colorWriteMask = 0xf;

	// //
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &blendAttachmentState;

	// input assembly //

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {};
	inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// depth stencil //
	VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo = {};
	depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	

	VkPipelineLayoutCreateInfo pipeLineLayoutCreateInfo = {};
	pipeLineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeLineLayoutCreateInfo.setLayoutCount = 1;
	pipeLineLayoutCreateInfo.pSetLayouts = &m_VkDescriptorSetLayout;
	/*pipeLineLayoutCreateInfo.pNext = nullptr;
	pipeLineLayoutCreateInfo.pPushConstantRanges = nullptr;
	pipeLineLayoutCreateInfo.pushConstantRangeCount = 0;*/

	if (vkCreatePipelineLayout(m_VkDevice, &pipeLineLayoutCreateInfo, nullptr, &m_VkPipelineLayout) != VK_SUCCESS )
	{
		printf("vkCreatePipelineLayout(m_VkDevice, &pipeLineLayoutCreateInfo, nullptr, &m_VkPipelineLayout)..\n");
		assert(false);

	}


	// now we can create the pipeline //

	VkGraphicsPipelineCreateInfo pipeLineCreateInfo = {};

	pipeLineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeLineCreateInfo.pMultisampleState = &multisampleCreateInfo; // multisample //
	pipeLineCreateInfo.renderPass = m_VkRenderPass; //render pass //
	pipeLineCreateInfo.pVertexInputState = &vertexInputCreateInfo; // vertex input
	pipeLineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo; // color blend //
	pipeLineCreateInfo.pRasterizationState = &rasterizationCreateInfo; //rasterization //
	pipeLineCreateInfo.pViewportState = &viewPortCreateInfo; // view port//
	pipeLineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
	pipeLineCreateInfo.layout = m_VkPipelineLayout;
	//pipeLineCreateInfo.pDepthStencilState =

	pipeLineCreateInfo.pStages = &shaderCreateInfo[0];
	pipeLineCreateInfo.stageCount = 2;
	pipeLineCreateInfo.basePipelineIndex = -1;
	pipeLineCreateInfo.subpass = 0;
	pipeLineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;



	VkResult res = vkCreateGraphicsPipelines(m_VkDevice, VK_NULL_HANDLE, 1, &pipeLineCreateInfo,NULL, &m_VKGraphicsPipeline );

	if (res != VK_SUCCESS)
	{
		printf("vkCreateGraphicsPipelines(m_VkDevice, VK_NULL_HANDLE, 1, &pipeLineCreateInfo,NULL, &m_VKGraphicsPipeline ) failed\n");
		assert(false);
		return -1;
	}





	

	return 1;


}
/**
 * @brief 
 * 
 */
void CVulkanInit::CreateVertexBuffer_09()
{
	VkPhysicalDevice physicalDevice = m_Devices.m_VkPhysicalDevices[m_iDeviceIndex];
	m_VertexBuffer.SetVulkanData(physicalDevice, m_VkDevice, m_VkCommandPool, m_VKPresentQue);
	
	/*m_VertexBuffer.AddVertex(CVertex(0.0f, -0.5f, 0.0f, 	0.25f, 0.0f, 0.99, 1.0f, 	0.0f, 0.0f));
	m_VertexBuffer.AddVertex(CVertex(0.5f, 0.5f, 0.0f, 		0.5f, 0.0f, 0.99, 1.0f,   	0.0f, 1.0f));
	m_VertexBuffer.AddVertex(CVertex(-0.5f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.99, 1.0f, 	1.0f, 1.0f));*/
	//m_VertexBuffer.AddVertex(CVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.25, 0.25f,0.25f, 1.0f));

	m_VertexBuffer.AddVertex(CVertex(0.0f, 0.0f, 0.0f, 	0.25f, 0.0f, 0.99, 1.0f, 	0.0f, 0.0f));
	m_VertexBuffer.AddVertex(CVertex(0.5f, 0.0f, 0.0f, 		0.5f, 0.0f, 0.99, 1.0f,   	1.0f, 0.0f));
	m_VertexBuffer.AddVertex(CVertex(0.5f, 0.5f, 0.0f, 	1.0f, 0.0f, 0.99, 1.0f, 	1.0f, 1.0f));

	m_VertexBuffer.AddVertex(CVertex(0.5f, 0.5f, 0.0f, 	1.0f, 0.0f, 0.99, 1.0f, 	1.0f, 1.0f));
	m_VertexBuffer.AddVertex(CVertex(0.0f, 0.5f, 0.0f, 		0.5f, 0.0f, 0.99, 1.0f,   	0.0f, 1.0f));
	m_VertexBuffer.AddVertex(CVertex(0.0f, 0.0f, 0.0f, 	0.25f, 0.0f, 0.99, 1.0f, 	0.0f, 0.0f));


	m_VertexBuffer.MoveDataToDevice();
	//m_VertexBuffer.AddVertex(CVertex())
	//m_VertexBuffer.AddVertex(CVertex())


}
/**.o
 * @brief 
 * 
 * @return int 
 */
int CVulkanInit::RecordCommandBuffer(unsigned int uiImageIndex)
{
	VkCommandBufferBeginInfo  commandBufferBeginInfo ={};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	//if (vkBeginCommandBuffer(m_VkCommandBuffers))

	VkClearColorValue clearClr = {9.0f/255.0f, 255.0f/255.0f ,10.0f/255.0f, 1.0f };
	/*VkClearValue clearValue[2] = {};
	clearValue[0].color = clearClr;
	clearValue[1].depthStencil = {1.0f, 0.0f};
	*/

	VkClearValue clearValue = {};
	clearValue.color = clearClr;
	//clearValue[1].depthStencil = {1.0f, 0.0f};

	VkImageSubresourceRange imageRange = {0};
	imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageRange.layerCount = 1;
	imageRange.levelCount = 1;

	VkRenderPassBeginInfo renderPassBeginInfo ={};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = m_VkRenderPass;
	//renderPassBeginInfo.framebuffer = m_VkFrameBuffers[0];
	renderPassBeginInfo.framebuffer = m_VkFrameBuffers[uiImageIndex];
	renderPassBeginInfo.renderArea.offset.x =0;
	renderPassBeginInfo.renderArea.offset.y =0;
	renderPassBeginInfo.renderArea.extent.height = m_iWindowHeight;
	renderPassBeginInfo.renderArea.extent.width = m_iWindowWidth;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;
	
	VkViewport viewPort = {0};
	viewPort.x = 0.0f;
	viewPort.y = 0.0f;

	viewPort.height = m_iWindowHeight;
	viewPort.width = m_iWindowWidth;
	viewPort.minDepth = 0.0f;
	viewPort.maxDepth = 1.0f;


	VkRect2D rectScissor = {0};
	rectScissor.offset.x = rectScissor.offset.y = 0.0f;
	rectScissor.extent.height = m_iWindowHeight;
	rectScissor.extent.width = m_iWindowWidth;


	UINT uiNumFrameBuffers = 1;//m_VkCommandBuffers.size();

	//for(UINT uiCount =0; uiCount < uiNumFrameBuffers; uiCount++ )

	//{
		VkCommandBuffer cmdBuff = m_VkCommandBuffers[uiImageIndex];
		if ( vkResetCommandBuffer(cmdBuff, 0) != VK_SUCCESS )
		{
			printf("vkResetCommandBuffer(m_VkCommandBuffers[uiCount]) .. failed \n");
			assert(false);
		}
		VkResult res = vkBeginCommandBuffer(cmdBuff, &commandBufferBeginInfo);
		if(res != VK_SUCCESS)
		{
			printf("vkBeginCommandBuffer(m_VkCommandBuffers[uiCount], &commandBufferBeginInfo) failed\n");
			assert(false);
			return -1;
		}


		renderPassBeginInfo.framebuffer = m_VkFrameBuffers[uiImageIndex];

		vkCmdBeginRenderPass(cmdBuff, &renderPassBeginInfo,VK_SUBPASS_CONTENTS_INLINE);

	//	printf("kkkk");

		vkCmdBindPipeline(cmdBuff,VK_PIPELINE_BIND_POINT_GRAPHICS,m_VKGraphicsPipeline);
		vkCmdSetScissor(cmdBuff,0,1, &rectScissor);
		vkCmdSetViewport(cmdBuff,0,1, &viewPort);
		VkDeviceSize offsets[] = {0};
		VkBuffer vkVertexBuffers[] = {m_VertexBuffer.GetVkBuffer()};
		vkCmdBindVertexBuffers(cmdBuff,0,1, vkVertexBuffers, offsets);
		vkCmdBindDescriptorSets(cmdBuff, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkPipelineLayout,0,1, &m_vVkDescriptorSets[0],0, nullptr );
		vkCmdDraw(cmdBuff,6,1,0,0);
		vkCmdEndRenderPass(cmdBuff);

		//vk
		res = vkEndCommandBuffer(cmdBuff);
		if(res != VK_SUCCESS)
		{
			printf("vkEndCommandBuffer(cmdBuff \n");
			assert(false);
			return -1;
		}

	//}

	return 1;


}

bool CVulkanInit::CreateSynchObjects_14()
{
	VkSemaphoreCreateInfo semphoreCreateInfo = {};
	semphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if ( vkCreateSemaphore(m_VkDevice, &semphoreCreateInfo, nullptr, &m_VkSemaphore_Wait) !=VK_SUCCESS )
	{
		printf("failed vkCreateSemaphore(m_VkDevice, &semphoreCreateInfo, nullptr, &m_VkSemaphore_Wait) \n");
		assert(false);
		return false;
	}

	if ( vkCreateSemaphore(m_VkDevice, &semphoreCreateInfo, nullptr, &m_VkSemaphore_RenderFinish) !=VK_SUCCESS )
	{
		printf("failed vkCreateSemaphore(m_VkDevice, &semphoreCreateInfo, nullptr, &m_VkSemaphore_RenderFinish) \n");
		assert(false);
		return false;
	}

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	if (vkCreateFence(m_VkDevice, &fenceCreateInfo, nullptr, &m_VkFence_Frame  ) != VK_SUCCESS )
	{
		printf("failed vkCreateFence(m_VkDevice, &fenceCreateInfo, nullptr, &m_VkFence_Frame  ) \n");
		assert(false);
		return false;
	}

	return true;


}
/**
 * @brief draws frame buffer
 * 
 * @return int 
 */
int CVulkanInit::Draw()
{
	UINT uiImageIndex = -1;
	//printf("before waitforfences ...\n");
	//fence //
	if (vkWaitForFences(m_VkDevice, 1, &m_VkFence_Frame, VK_TRUE, UINT64_MAX) != VK_SUCCESS )
	{
		printf("vkWaitForFences(m_VkDevice, 1, &m_VkFence_Frame, VK_TRUE, UINT64_MAX) failed.. \n");
		assert(false);
	}
	//printf("after waitforfences ... \n");

	if( vkAcquireNextImageKHR(m_VkDevice, m_VkSwapChain,UINT64_MAX, m_VkSemaphore_Wait/*VkQueueSubmit waits for this semaphore*/, VK_NULL_HANDLE, &uiImageIndex) != VK_SUCCESS )
	{
		printf("vkAcquireNextImageKHR(m_VkDevice, m_VkSwapChain,UINT64_MAX, NULL, NULL, &uiImageIndex) failed \n");
		assert(false);
		
	}

	if ( vkResetFences(m_VkDevice, 1, &m_VkFence_Frame) != VK_SUCCESS )
	{
		printf("vkResetFences(m_VkDevice, 1, &m_VkFence_Frame)...\n");
		assert(false);

	}
	
	RecordCommandBuffer(uiImageIndex);

	VkSubmitInfo	submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

	//submitInfo.pCommandBuffers = &cmdBuff

	submitInfo.pCommandBuffers = &m_VkCommandBuffers[uiImageIndex];
	submitInfo.commandBufferCount = 1;
	submitInfo.pWaitSemaphores = &m_VkSemaphore_Wait;
	submitInfo.waitSemaphoreCount = 1;

	submitInfo.pSignalSemaphores = &m_VkSemaphore_RenderFinish;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pWaitDstStageMask = waitStages;

	if (vkQueueSubmit(m_VKPresentQue, 1, &submitInfo, m_VkFence_Frame) != VK_SUCCESS)
	{
		printf("vkQueueSubmit(m_VKPresentQue, 1, &submitInfo, NULL) failed\n");
		assert(false);
	}

	VkPresentInfoKHR  presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pSwapchains = &m_VkSwapChain;
	presentInfo.swapchainCount = 1;
	presentInfo.pImageIndices = &uiImageIndex;

	presentInfo.pWaitSemaphores = &m_VkSemaphore_RenderFinish;
	presentInfo.waitSemaphoreCount =1;


	if (vkQueuePresentKHR(m_VKPresentQue, &presentInfo) != VK_SUCCESS )
	{
		printf("vkQueuePresentKHR(m_VKPresentQue, &presentInfo) failed \n");
		assert(false);
	}

	return 1;


}