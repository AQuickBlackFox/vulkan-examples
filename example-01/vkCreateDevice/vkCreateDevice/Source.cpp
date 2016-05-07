#include"vulkan\vulkan.h"
#include<iostream>
#include<vector>

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
	result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device);
	if (result == VK_SUCCESS) {
		std::cout << "Created a Vulkan Device" << std::endl;
	}
	else {
		std::cout << "Failed to create a Vulkan Device with flag: " << result<<std::endl;
	}


	std::cout << "Exiting.. " << std::endl;
}