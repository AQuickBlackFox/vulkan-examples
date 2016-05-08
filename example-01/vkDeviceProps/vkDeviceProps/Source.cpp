#include"vulkan\vulkan.h"
#include<iostream>
#include<vector>
#include<string>

std::vector<std::string> vkDeviceType = { "OTHER", "INTEGRATED_GPU", "DISCRETE_GPU", "VIRTUAL_GPU", "CPU" };


int main()
{
	VkResult result;
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VulkanQueues";
	appInfo.pEngineName = "GPU0";
	appInfo.apiVersion = VK_API_VERSION_1_0;

	std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = (uint32_t)enabledExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

	VkInstance pInstance;
	result = vkCreateInstance(&instanceCreateInfo, NULL, &pInstance);

	uint32_t gpuCount = 0;
	vkEnumeratePhysicalDevices(pInstance, &gpuCount, nullptr);
	std::cout << "Number of Vulkan capable GPUs: " << gpuCount << std::endl;

	if (gpuCount < 1)
	{
		std::cout << "No Vulkan capable GPUs found!" << std::endl;
		std::cout << "Exiting.." << std::endl;
		return 0;
	}

	/*
	Sample 02 starts here!
	*/

	std::vector<VkPhysicalDevice>physicalDevices(gpuCount);
	vkEnumeratePhysicalDevices(pInstance, &gpuCount, physicalDevices.data());

	uint32_t graphicsQueueId;

	for (uint32_t gpu = 0; gpu < gpuCount;gpu++) {
		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[gpu], &queueCount, NULL);
		std::cout << "Number of Queue Families on GPU " << gpu << " are: " << queueCount << std::endl;
		std::vector<VkQueueFamilyProperties> queueProps(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[gpu], &queueCount, queueProps.data());
		for (uint32_t queue = 0;queue < queueCount;queue++)
		{
			std::cout << "Number of Queues supported in this family: " << queueProps[queue].queueCount << std::endl;
			std::cout << "The capabilities of Queues are: " << queueProps[queue].queueFlags << std::endl;
			if (queueProps[queue].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsQueueId = queue;
			}
		}
	}

	/*
	Sample 03 starts here!
	*/
	std::cout << std::endl;
	for (int i = 0;i < gpuCount;i++) {
		const float queueProperties = 0.0f;
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = graphicsQueueId;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queueProperties;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = NULL;
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
		deviceCreateInfo.pEnabledFeatures = NULL;

		VkDevice device;
		result = vkCreateDevice(physicalDevices[i], &deviceCreateInfo, nullptr, &device);
		if (result == VK_SUCCESS) {
			std::cout << "Created a Vulkan Device on GPU " << i << std::endl;
		}
		else {
			std::cout << "Failed to create a Vulkan Device on GPU "<<i<<" with flag: " << result << std::endl;
		}

		/*
		Sample 04 starts here!
		*/

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevices[i], &deviceFeatures);
		std::cout << std::endl;
		std::cout << "On GPU " << i <<std::endl;
		std::cout <<"API Version   : "<< deviceProperties.apiVersion << std::endl;
		std::cout <<"Device ID     : "<< deviceProperties.deviceID << std::endl;
		std::cout <<"Device Name   : "<< deviceProperties.deviceName << std::endl;
		std::cout <<"Device Type   : "<< vkDeviceType[deviceProperties.deviceType] << std::endl;
		std::cout <<"Driver Version: "<< deviceProperties.driverVersion << std::endl;
		std::cout <<"Vendor ID     : "<< deviceProperties.vendorID << std::endl;
		//	std::cout << deviceProperties.limits << std::endl;
		std::cout << "== Features ==" << std::endl;
		std::cout << "Max Compute Shared Memory Size: "<< deviceProperties.limits.maxComputeSharedMemorySize << std::endl;
		std::cout << "Max Compute Work Group Count  : x = " << deviceProperties.limits.maxComputeWorkGroupCount[0];
		std::cout << ", y = " << deviceProperties.limits.maxComputeWorkGroupCount[1];
		std::cout << ", z = " << deviceProperties.limits.maxComputeWorkGroupCount[2]<<std::endl;
		std::cout << "Max Compute Invocations       : " << deviceProperties.limits.maxComputeWorkGroupInvocations << std::endl;
		std::cout << "Max Compute Work Group Size   : x = " << deviceProperties.limits.maxComputeWorkGroupSize[0];
		std::cout << ", y = " << deviceProperties.limits.maxComputeWorkGroupSize[1];
		std::cout << ", z = " << deviceProperties.limits.maxComputeWorkGroupSize[2] << std::endl;
	}
	std::cout << "Exiting.. " << std::endl;
}