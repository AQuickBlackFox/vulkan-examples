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
	uint32_t computeQueueId;

	for (uint32_t i = 0; i < gpuCount;i++) {
		uint32_t queueCount;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueCount, NULL);
		std::cout << "Number of Queue Families on GPU " << i << " are: " << queueCount << std::endl;
		std::vector<VkQueueFamilyProperties> queueProps(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueCount, queueProps.data());
		for (uint32_t queue = 0;queue < queueCount;queue++)
		{
			std::cout << "Number of Queues supported in this family: " << queueProps[queue].queueCount << std::endl;
			std::cout << "The capabilities of Queues are: " << queueProps[queue].queueFlags << std::endl;
			if (queueProps[queue].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				graphicsQueueId = queue;
			}
			if (queueProps[queue].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				computeQueueId = queue;
			}
		}

		/*
		Sample 03 starts here!
		*/
		std::cout << std::endl;
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
			std::cout << "Failed to create a Vulkan Device on GPU " << i << " with flag: " << result << std::endl;
		}

		/*
		Sample 04 starts here!
		*/

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		VkPhysicalDeviceMemoryProperties deviceMemoryProps;

		vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevices[i], &deviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &deviceMemoryProps);

		std::cout << std::endl;
		std::cout << "On GPU " << i << std::endl;
		std::cout << "API Version   : " << deviceProperties.apiVersion << std::endl;
		std::cout << "Device ID     : " << deviceProperties.deviceID << std::endl;
		std::cout << "Device Name   : " << deviceProperties.deviceName << std::endl;
		std::cout << "Device Type   : " << vkDeviceType[deviceProperties.deviceType] << std::endl;
		std::cout << "Driver Version: " << deviceProperties.driverVersion << std::endl;
		std::cout << "Vendor ID     : " << deviceProperties.vendorID << std::endl;
		//	std::cout << deviceProperties.limits << std::endl;
		std::cout << "== Features ==" << std::endl;
		std::cout << "Max Compute Shared Memory Size: " << deviceProperties.limits.maxComputeSharedMemorySize << std::endl;
		std::cout << "Max Compute Work Group Count  : x = " << deviceProperties.limits.maxComputeWorkGroupCount[0];
		std::cout << ", y = " << deviceProperties.limits.maxComputeWorkGroupCount[1];
		std::cout << ", z = " << deviceProperties.limits.maxComputeWorkGroupCount[2] << std::endl;
		std::cout << "Max Compute Invocations       : " << deviceProperties.limits.maxComputeWorkGroupInvocations << std::endl;
		std::cout << "Max Compute Work Group Size   : x = " << deviceProperties.limits.maxComputeWorkGroupSize[0];
		std::cout << ", y = " << deviceProperties.limits.maxComputeWorkGroupSize[1];
		std::cout << ", z = " << deviceProperties.limits.maxComputeWorkGroupSize[2] << std::endl;

		std::cout << "== Memory ==" << std::endl;
		uint32_t heapCount = deviceMemoryProps.memoryHeapCount;
		uint32_t typeCount = deviceMemoryProps.memoryTypeCount;
		std::cout << "Number of memory types supported: " << deviceMemoryProps.memoryTypeCount << std::endl;
		std::cout << "Memory types supported          : " << deviceMemoryProps.memoryHeapCount << std::endl;
		for (int j = 0;j < heapCount;j++)
		{
			std::cout << "For heap: " << j << std::endl;
			std::cout << deviceMemoryProps.memoryHeaps[i].size << std::endl;
			std::cout << deviceMemoryProps.memoryHeaps[i].flags << std::endl;
		}
		for (int j = 0;j < typeCount;j++)
		{
			std::cout << "For type: " << j << std::endl;
			std::cout << deviceMemoryProps.memoryTypes[i].heapIndex << std::endl;
			std::cout << deviceMemoryProps.memoryTypes[i].propertyFlags << std::endl;
		}

		VkQueue graphicsQueue;
		VkQueue computeQueue;
		vkGetDeviceQueue(device, graphicsQueueId, 0, &graphicsQueue);
		vkGetDeviceQueue(device, computeQueueId, 0, &computeQueue);

		std::cout << std::endl;
	}

	/*
	Sample 05 starts here
	We are selecting GPU0 for rendering
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

	VkPhysicalDevice pDevice = physicalDevices[0];
	VkDevice device;
	result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device);
	if (result == VK_SUCCESS) { std::cout << "Device Created" << std::endl; }
	VkCommandPoolCreateInfo cmdPoolInfo = {};
	VkCommandPool cmdPool;
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = graphicsQueueId;
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	result = vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool);
	if (result == VK_SUCCESS) { std::cout << "Created Command Pool" << std::endl; }

	VkCommandBuffer copyCommandBuffer = {};
	VkCommandBufferAllocateInfo bufAllocInfo = {};
	bufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	bufAllocInfo.commandPool = cmdPool;
	bufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	bufAllocInfo.commandBufferCount = 1;

	result = vkAllocateCommandBuffers(device, &bufAllocInfo, &copyCommandBuffer);
	if (result == VK_SUCCESS) { std::cout << "Command Buffer Created Successfully" << std::endl; }

	/*
	Sample 06 starts here!
	*/

	typedef struct {
		float x, y, z;
	} Vertex;

	std::vector<Vertex> Vertices = { {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f} };
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexMemory;
	VkBufferCreateInfo vertexBufferInfo = {};
	vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vertexBufferInfo.size = Vertices.size()*sizeof(Vertex);
	vertexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

	result = vkCreateBuffer(device, &vertexBufferInfo, nullptr, &vertexBuffer);
	if (result == VK_SUCCESS) { std::cout << "Buffer Creation Successful!" << std::endl; }

	VkMemoryRequirements memReqs = {};
	VkMemoryAllocateInfo memAlloc = {};
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memReqs);
	std::cout << memReqs.size << " " << memReqs.alignment << " " << (memReqs.memoryTypeBits & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)<< " ";
	std::cout << (memReqs.memoryTypeBits & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)<<" ";
	std::cout << (memReqs.memoryTypeBits & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)<<" ";
	std::cout << (memReqs.memoryTypeBits & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)<< " ";
	std::cout << (memReqs.memoryTypeBits & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) << std::endl;

	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAlloc.allocationSize = memReqs.size;

	VkPhysicalDeviceMemoryProperties pDevMemProp;
	vkGetPhysicalDeviceMemoryProperties(pDevice, &pDevMemProp);
	VkFlags properties = memReqs.memoryTypeBits;
	uint32_t typeBits = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	std::cout << memReqs.memoryTypeBits<< " "<< VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT << std::endl;
	uint32_t typeIndex;
	VkBool32 check = false;
	for (uint32_t i = 0;i < 32;i++) {
		if ((typeBits & 1) == 1)
		{
			if ((pDevMemProp.memoryTypes[i].propertyFlags & properties) == properties) {
				typeIndex = i;
				break;
			}
		}
		typeBits >>= 1;
	}

	memAlloc.memoryTypeIndex = typeBits;
	std::cout << typeBits << std::endl;
	void *data;
	result = vkAllocateMemory(device, &memAlloc, nullptr, &vertexMemory);
	if (result == VK_SUCCESS) { std::cout << "Device Memory Allocation Success" << std::endl; }

	result = vkMapMemory(device, vertexMemory, 0, memAlloc.allocationSize, 0, &data);
	memcpy(data, Vertices.data(), memAlloc.allocationSize);
	vkUnmapMemory(device, vertexMemory);

	result = vkBindBufferMemory(device, vertexBuffer, vertexMemory, 0);
	if(result == VK_SUCCESS){ std::cout << "Binded Buffer" << std::endl; }

	vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	vkCreateBuffer(device, &vertexBufferInfo, nullptr, &vertexBuffer);
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memReqs);
	memAlloc.allocationSize = memReqs.size;
	vkAllocateMemory(device, &memAlloc, nullptr, &vertexMemory);
	vkBindBufferMemory(device, vertexBuffer, vertexMemory, 0);

	std::cout << "Exiting.. " << std::endl;
}