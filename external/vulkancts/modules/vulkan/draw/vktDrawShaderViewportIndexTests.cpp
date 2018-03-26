/*------------------------------------------------------------------------
 * Vulkan Conformance Tests
 * ------------------------
 *
 * Copyright (c) 2018 The Khronos Group Inc.
 * Copyright (c) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief Use of gl_ViewportIndex in Vertex Shader (part of VK_EXT_ShaderViewportIndexLayer)
 *//*--------------------------------------------------------------------*/

#include "vktDrawShaderViewportIndexTests.hpp"

#include "vktDrawBaseClass.hpp"
#include "vktTestCaseUtil.hpp"

#include "vkDefs.hpp"
#include "vkRef.hpp"
#include "vkRefUtil.hpp"
#include "vkTypeUtil.hpp"
#include "vkMemUtil.hpp"
#include "vkPrograms.hpp"
#include "vkImageUtil.hpp"
#include "vkQueryUtil.hpp"

#include "tcuTestLog.hpp"
#include "tcuVector.hpp"
#include "tcuImageCompare.hpp"
#include "tcuTextureUtil.hpp"

#include "deUniquePtr.hpp"
#include "deMath.h"

#include <vector>

namespace vkt
{
namespace Draw
{
using namespace vk;
using de::UniquePtr;
using de::MovePtr;
using de::SharedPtr;
using tcu::Vec4;
using tcu::Vec2;
using tcu::UVec2;
using tcu::UVec4;

namespace
{

enum Constants
{
	MIN_MAX_VIEWPORTS = 16,		//!< Minimum number of viewports for an implementation supporting multiViewport.
};

template<typename T>
inline VkDeviceSize sizeInBytes(const std::vector<T>& vec)
{
	return vec.size() * sizeof(vec[0]);
}

VkBufferCreateInfo makeBufferCreateInfo (const VkDeviceSize			bufferSize,
										 const VkBufferUsageFlags	usage)
{
	const VkBufferCreateInfo bufferCreateInfo =
	{
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,	// VkStructureType		sType;
		DE_NULL,								// const void*			pNext;
		(VkBufferCreateFlags)0,					// VkBufferCreateFlags	flags;
		bufferSize,								// VkDeviceSize			size;
		usage,									// VkBufferUsageFlags	usage;
		VK_SHARING_MODE_EXCLUSIVE,				// VkSharingMode		sharingMode;
		0u,										// deUint32				queueFamilyIndexCount;
		DE_NULL,								// const deUint32*		pQueueFamilyIndices;
	};
	return bufferCreateInfo;
}

Move<VkPipelineLayout> makePipelineLayout (const DeviceInterface&		vk,
										   const VkDevice				device)
{
	const VkPipelineLayoutCreateInfo info =
	{
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,		// VkStructureType				sType;
		DE_NULL,											// const void*					pNext;
		(VkPipelineLayoutCreateFlags)0,						// VkPipelineLayoutCreateFlags	flags;
		0u,													// deUint32						setLayoutCount;
		DE_NULL,											// const VkDescriptorSetLayout*	pSetLayouts;
		0u,													// deUint32						pushConstantRangeCount;
		DE_NULL,											// const VkPushConstantRange*	pPushConstantRanges;
	};
	return createPipelineLayout(vk, device, &info);
}

Move<VkImageView> makeImageView (const DeviceInterface&			vk,
								 const VkDevice					vkDevice,
								 const VkImage					image,
								 const VkImageViewType			viewType,
								 const VkFormat					format,
								 const VkImageSubresourceRange	subresourceRange)
{
	const VkImageViewCreateInfo imageViewParams =
	{
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,		// VkStructureType			sType;
		DE_NULL,										// const void*				pNext;
		(VkImageViewCreateFlags)0,						// VkImageViewCreateFlags	flags;
		image,											// VkImage					image;
		viewType,										// VkImageViewType			viewType;
		format,											// VkFormat					format;
		makeComponentMappingRGBA(),						// VkComponentMapping		components;
		subresourceRange,								// VkImageSubresourceRange	subresourceRange;
	};
	return createImageView(vk, vkDevice, &imageViewParams);
}

void beginCommandBuffer (const DeviceInterface& vk, const VkCommandBuffer commandBuffer)
{
	const VkCommandBufferBeginInfo info =
	{
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	// VkStructureType                          sType;
		DE_NULL,										// const void*                              pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	// VkCommandBufferUsageFlags                flags;
		DE_NULL,										// const VkCommandBufferInheritanceInfo*    pInheritanceInfo;
	};
	VK_CHECK(vk.beginCommandBuffer(commandBuffer, &info));
}

void submitCommandsAndWait (const DeviceInterface&	vk,
							const VkDevice			device,
							const VkQueue			queue,
							const VkCommandBuffer	commandBuffer)
{
	const Unique<VkFence> fence(createFence(vk, device));

	const VkSubmitInfo submitInfo =
	{
		VK_STRUCTURE_TYPE_SUBMIT_INFO,		// VkStructureType                sType;
		DE_NULL,							// const void*                    pNext;
		0u,									// uint32_t                       waitSemaphoreCount;
		DE_NULL,							// const VkSemaphore*             pWaitSemaphores;
		DE_NULL,							// const VkPipelineStageFlags*    pWaitDstStageMask;
		1u,									// uint32_t                       commandBufferCount;
		&commandBuffer,						// const VkCommandBuffer*         pCommandBuffers;
		0u,									// uint32_t                       signalSemaphoreCount;
		DE_NULL,							// const VkSemaphore*             pSignalSemaphores;
	};
	VK_CHECK(vk.queueSubmit(queue, 1u, &submitInfo, *fence));
	VK_CHECK(vk.waitForFences(device, 1u, &fence.get(), DE_TRUE, ~0ull));
}

Move<VkFramebuffer> makeFramebuffer (const DeviceInterface&		vk,
									 const VkDevice				device,
									 const VkRenderPass			renderPass,
									 const deUint32				attachmentCount,
									 const VkImageView*			pAttachments,
									 const deUint32				width,
									 const deUint32				height,
									 const deUint32				layers = 1u)
{
	const VkFramebufferCreateInfo framebufferInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,		// VkStructureType                             sType;
		DE_NULL,										// const void*                                 pNext;
		(VkFramebufferCreateFlags)0,					// VkFramebufferCreateFlags                    flags;
		renderPass,										// VkRenderPass                                renderPass;
		attachmentCount,								// uint32_t                                    attachmentCount;
		pAttachments,									// const VkImageView*                          pAttachments;
		width,											// uint32_t                                    width;
		height,											// uint32_t                                    height;
		layers,											// uint32_t                                    layers;
	};

	return createFramebuffer(vk, device, &framebufferInfo);
}

MovePtr<Allocation> bindImage (const DeviceInterface& vk, const VkDevice device, Allocator& allocator, const VkImage image, const MemoryRequirement requirement)
{
	MovePtr<Allocation> alloc = allocator.allocate(getImageMemoryRequirements(vk, device, image), requirement);
	VK_CHECK(vk.bindImageMemory(device, image, alloc->getMemory(), alloc->getOffset()));
	return alloc;
}

inline vk::Move<vk::VkImage> makeImage (const vk::DeviceInterface& vk, const vk::VkDevice device, const vk::VkImageCreateInfo& createInfo)
{
	return createImage(vk, device, &createInfo);
}

VkImageCreateInfo makeImageCreateInfo (const VkFormat format, const UVec2& size, VkImageUsageFlags usage)
{
	const VkImageCreateInfo imageParams =
	{
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,			// VkStructureType			sType;
		DE_NULL,										// const void*				pNext;
		(VkImageCreateFlags)0,							// VkImageCreateFlags		flags;
		VK_IMAGE_TYPE_2D,								// VkImageType				imageType;
		format,											// VkFormat					format;
		makeExtent3D(size.x(), size.y(), 1),			// VkExtent3D				extent;
		1u,												// deUint32					mipLevels;
		1u,												// deUint32					arrayLayers;
		VK_SAMPLE_COUNT_1_BIT,							// VkSampleCountFlagBits	samples;
		VK_IMAGE_TILING_OPTIMAL,						// VkImageTiling			tiling;
		usage,											// VkImageUsageFlags		usage;
		VK_SHARING_MODE_EXCLUSIVE,						// VkSharingMode			sharingMode;
		0u,												// deUint32					queueFamilyIndexCount;
		DE_NULL,										// const deUint32*			pQueueFamilyIndices;
		VK_IMAGE_LAYOUT_UNDEFINED,						// VkImageLayout			initialLayout;
	};
	return imageParams;
}

//! A single-attachment, single-subpass render pass.
Move<VkRenderPass> makeRenderPass (const DeviceInterface&	vk,
								   const VkDevice			device,
								   const VkFormat			colorFormat)
{
	const VkAttachmentDescription colorAttachmentDescription =
	{
		(VkAttachmentDescriptionFlags)0,					// VkAttachmentDescriptionFlags		flags;
		colorFormat,										// VkFormat							format;
		VK_SAMPLE_COUNT_1_BIT,								// VkSampleCountFlagBits			samples;
		VK_ATTACHMENT_LOAD_OP_CLEAR,						// VkAttachmentLoadOp				loadOp;
		VK_ATTACHMENT_STORE_OP_STORE,						// VkAttachmentStoreOp				storeOp;
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,					// VkAttachmentLoadOp				stencilLoadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,					// VkAttachmentStoreOp				stencilStoreOp;
		VK_IMAGE_LAYOUT_UNDEFINED,							// VkImageLayout					initialLayout;
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,			// VkImageLayout					finalLayout;
	};

	const VkAttachmentReference colorAttachmentRef =
	{
		0u,													// deUint32			attachment;
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// VkImageLayout	layout;
	};

	const VkSubpassDescription subpassDescription =
	{
		(VkSubpassDescriptionFlags)0,						// VkSubpassDescriptionFlags		flags;
		VK_PIPELINE_BIND_POINT_GRAPHICS,					// VkPipelineBindPoint				pipelineBindPoint;
		0u,													// deUint32							inputAttachmentCount;
		DE_NULL,											// const VkAttachmentReference*		pInputAttachments;
		1u,													// deUint32							colorAttachmentCount;
		&colorAttachmentRef,								// const VkAttachmentReference*		pColorAttachments;
		DE_NULL,											// const VkAttachmentReference*		pResolveAttachments;
		DE_NULL,											// const VkAttachmentReference*		pDepthStencilAttachment;
		0u,													// deUint32							preserveAttachmentCount;
		DE_NULL												// const deUint32*					pPreserveAttachments;
	};

	const VkRenderPassCreateInfo renderPassInfo =
	{
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,			// VkStructureType					sType;
		DE_NULL,											// const void*						pNext;
		(VkRenderPassCreateFlags)0,							// VkRenderPassCreateFlags			flags;
		1u,													// deUint32							attachmentCount;
		&colorAttachmentDescription,						// const VkAttachmentDescription*	pAttachments;
		1u,													// deUint32							subpassCount;
		&subpassDescription,								// const VkSubpassDescription*		pSubpasses;
		0u,													// deUint32							dependencyCount;
		DE_NULL												// const VkSubpassDependency*		pDependencies;
	};

	return createRenderPass(vk, device, &renderPassInfo);
}

Move<VkPipeline> makeGraphicsPipeline (const DeviceInterface&		vk,
									   const VkDevice				device,
									   const VkPipelineLayout		pipelineLayout,
									   const VkRenderPass			renderPass,
									   const VkShaderModule			vertexModule,
									   const VkShaderModule			fragmentModule,
									   const UVec2					renderSize,
									   const int					numViewports,
									   const std::vector<UVec4>		cells)
{
	const VkVertexInputBindingDescription vertexInputBindingDescription =
	{
		0u,								// uint32_t				binding;
		sizeof(PositionColorVertex),	// uint32_t				stride;
		VK_VERTEX_INPUT_RATE_VERTEX,	// VkVertexInputRate	inputRate;
	};

	const VkVertexInputAttributeDescription vertexInputAttributeDescriptions[] =
	{
		{
			0u,									// uint32_t			location;
			0u,									// uint32_t			binding;
			VK_FORMAT_R32G32B32A32_SFLOAT,		// VkFormat			format;
			0u,									// uint32_t			offset;
		},
		{
			1u,									// uint32_t			location;
			0u,									// uint32_t			binding;
			VK_FORMAT_R32G32B32A32_SFLOAT,		// VkFormat			format;
			sizeof(Vec4),						// uint32_t			offset;
		},
	};

	const VkPipelineVertexInputStateCreateInfo vertexInputStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,		// VkStructureType                             sType;
		DE_NULL,														// const void*                                 pNext;
		(VkPipelineVertexInputStateCreateFlags)0,						// VkPipelineVertexInputStateCreateFlags       flags;
		1u,																// uint32_t                                    vertexBindingDescriptionCount;
		&vertexInputBindingDescription,									// const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
		DE_LENGTH_OF_ARRAY(vertexInputAttributeDescriptions),			// uint32_t                                    vertexAttributeDescriptionCount;
		vertexInputAttributeDescriptions,								// const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
	};

	const VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,	// VkStructureType                             sType;
		DE_NULL,														// const void*                                 pNext;
		(VkPipelineInputAssemblyStateCreateFlags)0,						// VkPipelineInputAssemblyStateCreateFlags     flags;
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,							// VkPrimitiveTopology                         topology;
		VK_FALSE,														// VkBool32                                    primitiveRestartEnable;
	};

	DE_ASSERT(numViewports == static_cast<int>(cells.size()));

	std::vector<VkViewport> viewports;
	viewports.reserve(numViewports);

	std::vector<VkRect2D> rectScissors;
	rectScissors.reserve(numViewports);

	for (std::vector<UVec4>::const_iterator it = cells.begin(); it != cells.end(); ++it) {
		const VkViewport viewport = makeViewport(float(it->x()), float(it->y()), float(it->z()), float(it->w()), 0.0f, 1.0f);
		viewports.push_back(viewport);
		const VkRect2D rect = { {0, 0}, {renderSize.x(), renderSize.y()} };
		rectScissors.push_back(rect);
	}

	const VkPipelineViewportStateCreateInfo pipelineViewportStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,			// VkStructureType                             sType;
		DE_NULL,														// const void*                                 pNext;
		(VkPipelineViewportStateCreateFlags)0,							// VkPipelineViewportStateCreateFlags          flags;
		static_cast<deUint32>(numViewports),							// uint32_t                                    viewportCount;
		&viewports[0],													// const VkViewport*                           pViewports;
		static_cast<deUint32>(numViewports),							// uint32_t                                    scissorCount;
		&rectScissors[0],												// const VkRect2D*                             pScissors;
	};

	const VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,		// VkStructureType                          sType;
		DE_NULL,														// const void*                              pNext;
		(VkPipelineRasterizationStateCreateFlags)0,						// VkPipelineRasterizationStateCreateFlags  flags;
		VK_FALSE,														// VkBool32                                 depthClampEnable;
		VK_FALSE,														// VkBool32                                 rasterizerDiscardEnable;
		VK_POLYGON_MODE_FILL,											// VkPolygonMode							polygonMode;
		VK_CULL_MODE_NONE,												// VkCullModeFlags							cullMode;
		VK_FRONT_FACE_COUNTER_CLOCKWISE,								// VkFrontFace								frontFace;
		VK_FALSE,														// VkBool32									depthBiasEnable;
		0.0f,															// float									depthBiasConstantFactor;
		0.0f,															// float									depthBiasClamp;
		0.0f,															// float									depthBiasSlopeFactor;
		1.0f,															// float									lineWidth;
	};

	const VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,		// VkStructureType							sType;
		DE_NULL,														// const void*								pNext;
		(VkPipelineMultisampleStateCreateFlags)0,						// VkPipelineMultisampleStateCreateFlags	flags;
		VK_SAMPLE_COUNT_1_BIT,											// VkSampleCountFlagBits					rasterizationSamples;
		VK_FALSE,														// VkBool32									sampleShadingEnable;
		0.0f,															// float									minSampleShading;
		DE_NULL,														// const VkSampleMask*						pSampleMask;
		VK_FALSE,														// VkBool32									alphaToCoverageEnable;
		VK_FALSE														// VkBool32									alphaToOneEnable;
	};

	const VkStencilOpState stencilOpState = makeStencilOpState(
		VK_STENCIL_OP_KEEP,				// stencil fail
		VK_STENCIL_OP_KEEP,				// depth & stencil pass
		VK_STENCIL_OP_KEEP,				// depth only fail
		VK_COMPARE_OP_ALWAYS,			// compare op
		0u,								// compare mask
		0u,								// write mask
		0u);							// reference

	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,		// VkStructureType							sType;
		DE_NULL,														// const void*								pNext;
		(VkPipelineDepthStencilStateCreateFlags)0,						// VkPipelineDepthStencilStateCreateFlags	flags;
		VK_FALSE,														// VkBool32									depthTestEnable;
		VK_FALSE,														// VkBool32									depthWriteEnable;
		VK_COMPARE_OP_LESS,												// VkCompareOp								depthCompareOp;
		VK_FALSE,														// VkBool32									depthBoundsTestEnable;
		VK_FALSE,														// VkBool32									stencilTestEnable;
		stencilOpState,													// VkStencilOpState							front;
		stencilOpState,													// VkStencilOpState							back;
		0.0f,															// float									minDepthBounds;
		1.0f,															// float									maxDepthBounds;
	};

	const VkColorComponentFlags					colorComponentsAll					= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	const VkPipelineColorBlendAttachmentState	pipelineColorBlendAttachmentState	=
	{
		VK_FALSE,						// VkBool32					blendEnable;
		VK_BLEND_FACTOR_ONE,			// VkBlendFactor			srcColorBlendFactor;
		VK_BLEND_FACTOR_ZERO,			// VkBlendFactor			dstColorBlendFactor;
		VK_BLEND_OP_ADD,				// VkBlendOp				colorBlendOp;
		VK_BLEND_FACTOR_ONE,			// VkBlendFactor			srcAlphaBlendFactor;
		VK_BLEND_FACTOR_ZERO,			// VkBlendFactor			dstAlphaBlendFactor;
		VK_BLEND_OP_ADD,				// VkBlendOp				alphaBlendOp;
		colorComponentsAll,				// VkColorComponentFlags	colorWriteMask;
	};

	const VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateInfo =
	{
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,		// VkStructureType								sType;
		DE_NULL,														// const void*									pNext;
		(VkPipelineColorBlendStateCreateFlags)0,						// VkPipelineColorBlendStateCreateFlags			flags;
		VK_FALSE,														// VkBool32										logicOpEnable;
		VK_LOGIC_OP_COPY,												// VkLogicOp									logicOp;
		1u,																// deUint32										attachmentCount;
		&pipelineColorBlendAttachmentState,								// const VkPipelineColorBlendAttachmentState*	pAttachments;
		{ 0.0f, 0.0f, 0.0f, 0.0f },										// float										blendConstants[4];
	};

	const VkPipelineShaderStageCreateInfo pShaderStages[] =
	{
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,		// VkStructureType						sType;
			DE_NULL,													// const void*							pNext;
			(VkPipelineShaderStageCreateFlags)0,						// VkPipelineShaderStageCreateFlags		flags;
			VK_SHADER_STAGE_VERTEX_BIT,									// VkShaderStageFlagBits				stage;
			vertexModule,												// VkShaderModule						module;
			"main",														// const char*							pName;
			DE_NULL,													// const VkSpecializationInfo*			pSpecializationInfo;
		},
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,		// VkStructureType						sType;
			DE_NULL,													// const void*							pNext;
			(VkPipelineShaderStageCreateFlags)0,						// VkPipelineShaderStageCreateFlags		flags;
			VK_SHADER_STAGE_FRAGMENT_BIT,								// VkShaderStageFlagBits				stage;
			fragmentModule,												// VkShaderModule						module;
			"main",														// const char*							pName;
			DE_NULL,													// const VkSpecializationInfo*			pSpecializationInfo;
		},
	};

	const VkGraphicsPipelineCreateInfo graphicsPipelineInfo =
	{
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,	// VkStructureType									sType;
		DE_NULL,											// const void*										pNext;
		(VkPipelineCreateFlags)0,							// VkPipelineCreateFlags							flags;
		DE_LENGTH_OF_ARRAY(pShaderStages),					// deUint32											stageCount;
		pShaderStages,										// const VkPipelineShaderStageCreateInfo*			pStages;
		&vertexInputStateInfo,								// const VkPipelineVertexInputStateCreateInfo*		pVertexInputState;
		&pipelineInputAssemblyStateInfo,					// const VkPipelineInputAssemblyStateCreateInfo*	pInputAssemblyState;
		DE_NULL,											// const VkPipelineTessellationStateCreateInfo*		pTessellationState;
		&pipelineViewportStateInfo,							// const VkPipelineViewportStateCreateInfo*			pViewportState;
		&pipelineRasterizationStateInfo,					// const VkPipelineRasterizationStateCreateInfo*	pRasterizationState;
		&pipelineMultisampleStateInfo,						// const VkPipelineMultisampleStateCreateInfo*		pMultisampleState;
		&pipelineDepthStencilStateInfo,						// const VkPipelineDepthStencilStateCreateInfo*		pDepthStencilState;
		&pipelineColorBlendStateInfo,						// const VkPipelineColorBlendStateCreateInfo*		pColorBlendState;
		DE_NULL,											// const VkPipelineDynamicStateCreateInfo*			pDynamicState;
		pipelineLayout,										// VkPipelineLayout									layout;
		renderPass,											// VkRenderPass										renderPass;
		0u,													// deUint32											subpass;
		DE_NULL,											// VkPipeline										basePipelineHandle;
		0,													// deInt32											basePipelineIndex;
	};

	return createGraphicsPipeline(vk, device, DE_NULL, &graphicsPipelineInfo);
}

std::vector<UVec4> generateGrid (const int numCells, const UVec2& renderSize)
{
	const int numCols		= deCeilFloatToInt32(deFloatSqrt(static_cast<float>(numCells)));
	const int numRows		= deCeilFloatToInt32(static_cast<float>(numCells) / static_cast<float>(numCols));
	const int rectWidth		= renderSize.x() / numCols;
	const int rectHeight	= renderSize.y() / numRows;

	std::vector<UVec4> cells;
	cells.reserve(numCells);

	int x = 0;
	int y = 0;

	for (int cellNdx = 0; cellNdx < numCells; ++cellNdx)
	{
		const bool nextRow = (cellNdx != 0) && (cellNdx % numCols == 0);
		if (nextRow)
		{
			x  = 0;
			y += rectHeight;
		}

		cells.push_back(UVec4(x, y, rectWidth, rectHeight));

		x += rectWidth;
	}

	return cells;
}

std::vector<Vec4> generateColors (const int numColors)
{
	const Vec4 colors[] =
	{
		Vec4(0.18f, 0.42f, 0.17f, 1.0f),
		Vec4(0.29f, 0.62f, 0.28f, 1.0f),
		Vec4(0.59f, 0.84f, 0.44f, 1.0f),
		Vec4(0.96f, 0.95f, 0.72f, 1.0f),
		Vec4(0.94f, 0.55f, 0.39f, 1.0f),
		Vec4(0.82f, 0.19f, 0.12f, 1.0f),
		Vec4(0.46f, 0.15f, 0.26f, 1.0f),
		Vec4(0.24f, 0.14f, 0.24f, 1.0f),
		Vec4(0.49f, 0.31f, 0.26f, 1.0f),
		Vec4(0.78f, 0.52f, 0.33f, 1.0f),
		Vec4(0.94f, 0.82f, 0.31f, 1.0f),
		Vec4(0.98f, 0.65f, 0.30f, 1.0f),
		Vec4(0.22f, 0.65f, 0.53f, 1.0f),
		Vec4(0.67f, 0.81f, 0.91f, 1.0f),
		Vec4(0.43f, 0.44f, 0.75f, 1.0f),
		Vec4(0.26f, 0.24f, 0.48f, 1.0f),
	};

	DE_ASSERT(numColors <= DE_LENGTH_OF_ARRAY(colors));

	return std::vector<Vec4>(colors, colors + numColors);
}

//! Renders a colorful grid of rectangles.
tcu::TextureLevel generateReferenceImage (const tcu::TextureFormat	format,
										  const UVec2&				renderSize,
										  const Vec4&				clearColor,
										  const std::vector<UVec4>&	cells,
										  const std::vector<Vec4>&	cellColors)
{
	DE_ASSERT(cells.size() == cellColors.size());

	tcu::TextureLevel image(format, renderSize.x(), renderSize.y());
	tcu::clear(image.getAccess(), clearColor);

	for (std::size_t i = 0; i < cells.size(); ++i)
	{
		const UVec4& cell = cells[i];
		tcu::clear(
			tcu::getSubregion(image.getAccess(), cell.x(), cell.y(), cell.z(), cell.w()),
			cellColors[i]);
	}

	return image;
}

void initPrograms (SourceCollections& programCollection, const int numViewports)
{
	DE_UNREF(numViewports);

	// Vertex shader
	{
		std::ostringstream src;
		src << glu::getGLSLVersionDeclaration(glu::GLSL_VERSION_450) << "\n"
			<< "#extension GL_ARB_shader_viewport_layer_array : require\n"
			<< "\n"
			<< "layout(location = 0) in  vec4 in_position;\n"
			<< "layout(location = 1) in  vec4 in_color;\n"
			<< "layout(location = 0) out vec4 out_color;\n"
			<< "\n"
			<< "void main(void)\n"
			<< "{\n"
			<< "    gl_ViewportIndex = gl_VertexIndex / 6;\n"
			<< "    gl_Position = in_position;\n"
			<< "    out_color = in_color;\n"
			<< "}\n";

		programCollection.glslSources.add("vert") << glu::VertexSource(src.str());
	}

	// Fragment shader
	{
		std::ostringstream src;
		src << glu::getGLSLVersionDeclaration(glu::GLSL_VERSION_450) << "\n"
			<< "\n"
			<< "layout(location = 0) in  vec4 in_color;\n"
			<< "layout(location = 0) out vec4 out_color;\n"
			<< "\n"
			<< "void main(void)\n"
			<< "{\n"
			<< "    out_color = in_color;\n"
			<< "}\n";

		programCollection.glslSources.add("frag") << glu::FragmentSource(src.str());
	}
}

std::vector<PositionColorVertex> generateVertices (const std::vector<Vec4>& colors)
{
	// Two triangles for each color (viewport).
	std::size_t total = colors.size() * 6;

	std::vector<PositionColorVertex> result;
	result.reserve(total);

	for (std::size_t i = 0; i < total; ++i)
	{
		Vec4 pos;
		pos.z() = 0.0;
		pos.w() = 1.0;

		switch (i % 6) {
		case 0: pos.xy() = Vec2(-1.0,  1.0); break;
		case 1: pos.xy() = Vec2( 1.0,  1.0); break;
		case 2: pos.xy() = Vec2(-1.0, -1.0); break;
		case 3: pos.xy() = Vec2( 1.0, -1.0); break;
		case 4: pos.xy() = Vec2( 1.0,  1.0); break;
		case 5: pos.xy() = Vec2(-1.0, -1.0); break;
		}

		result.push_back(PositionColorVertex(pos, colors[i/6]));
	}

	return result;
}

// Renderer generates two triangles per viewport, each pair using a different color. The
// numViewports are positioned to form a grid.
class Renderer
{
public:
	Renderer (Context&						context,
			  const UVec2&					renderSize,
			  const int						numViewports,
			  const std::vector<UVec4>&		cells,
			  const VkFormat				colorFormat,
			  const Vec4&					clearColor,
			  const std::vector<Vec4>&		colors)
		: m_renderSize				(renderSize)
		, m_colorFormat				(colorFormat)
		, m_colorSubresourceRange	(makeImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u))
		, m_clearColor				(clearColor)
		, m_numViewports			(numViewports)
		, m_vertices				(generateVertices(colors))
	{
		const DeviceInterface&		vk					= context.getDeviceInterface();
		const VkDevice				device				= context.getDevice();
		const deUint32				queueFamilyIndex	= context.getUniversalQueueFamilyIndex();
		Allocator&					allocator			= context.getDefaultAllocator();
		const VkDeviceSize			vertexBufferSize    = sizeInBytes(m_vertices);

		m_colorImage		= makeImage					(vk, device, makeImageCreateInfo(m_colorFormat, m_renderSize, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT));
		m_colorImageAlloc	= bindImage					(vk, device, allocator, *m_colorImage, MemoryRequirement::Any);
		m_colorAttachment	= makeImageView				(vk, device, *m_colorImage, VK_IMAGE_VIEW_TYPE_2D, m_colorFormat, m_colorSubresourceRange);

		m_vertexBuffer		= Buffer::createAndAlloc	(vk, device, makeBufferCreateInfo(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT), allocator, MemoryRequirement::HostVisible);

		{
			deMemcpy(m_vertexBuffer->getBoundMemory().getHostPtr(), &m_vertices[0], static_cast<std::size_t>(vertexBufferSize));
			flushMappedMemoryRange(vk, device,
								   m_vertexBuffer->getBoundMemory().getMemory(),
								   m_vertexBuffer->getBoundMemory().getOffset(),
								   vertexBufferSize);
		}

		m_vertexModule		= createShaderModule	(vk, device, context.getBinaryCollection().get("vert"), 0u);
		m_fragmentModule	= createShaderModule	(vk, device, context.getBinaryCollection().get("frag"), 0u);
		m_renderPass		= makeRenderPass		(vk, device, m_colorFormat);
		m_framebuffer		= makeFramebuffer		(vk, device, *m_renderPass, 1u, &m_colorAttachment.get(),
													 static_cast<deUint32>(m_renderSize.x()),  static_cast<deUint32>(m_renderSize.y()));
		m_pipelineLayout	= makePipelineLayout	(vk, device);
		m_pipeline			= makeGraphicsPipeline	(vk, device, *m_pipelineLayout, *m_renderPass, *m_vertexModule, *m_fragmentModule,
													 m_renderSize, m_numViewports, cells);
		m_cmdPool			= createCommandPool		(vk, device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queueFamilyIndex);
		m_cmdBuffer			= allocateCommandBuffer	(vk, device, *m_cmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	}

	void draw (Context& context, const VkBuffer colorBuffer) const
	{
		const DeviceInterface&		vk			= context.getDeviceInterface();
		const VkDevice				device		= context.getDevice();
		const VkQueue				queue		= context.getUniversalQueue();

		beginCommandBuffer(vk, *m_cmdBuffer);

		const VkClearValue			clearValue	= makeClearValueColor(m_clearColor);
		const VkRect2D				renderArea	=
		{
			makeOffset2D(0, 0),
			makeExtent2D(m_renderSize.x(), m_renderSize.y()),
		};
		const VkRenderPassBeginInfo renderPassBeginInfo =
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,		// VkStructureType         sType;
			DE_NULL,										// const void*             pNext;
			*m_renderPass,									// VkRenderPass            renderPass;
			*m_framebuffer,									// VkFramebuffer           framebuffer;
			renderArea,										// VkRect2D                renderArea;
			1u,												// uint32_t                clearValueCount;
			&clearValue,									// const VkClearValue*     pClearValues;
		};
		vk.cmdBeginRenderPass(*m_cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		vk.cmdBindPipeline(*m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
		{
			const VkBuffer vertexBuffer = m_vertexBuffer->object();
			const VkDeviceSize vertexBufferOffset = 0ull;
			vk.cmdBindVertexBuffers(*m_cmdBuffer, 0u, 1u, &vertexBuffer, &vertexBufferOffset);
		}
		vk.cmdDraw(*m_cmdBuffer, static_cast<deUint32>(m_numViewports * 6), 1u, 0u, 0u);	// two triangles per viewport
		vk.cmdEndRenderPass(*m_cmdBuffer);

		// Prepare color image for copy
		{
			const VkImageMemoryBarrier barriers[] =
			{
				{
					VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,						// VkStructureType			sType;
					DE_NULL,													// const void*				pNext;
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,						// VkAccessFlags			outputMask;
					VK_ACCESS_TRANSFER_READ_BIT,								// VkAccessFlags			inputMask;
					VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,					// VkImageLayout			oldLayout;
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,						// VkImageLayout			newLayout;
					VK_QUEUE_FAMILY_IGNORED,									// deUint32					srcQueueFamilyIndex;
					VK_QUEUE_FAMILY_IGNORED,									// deUint32					destQueueFamilyIndex;
					*m_colorImage,												// VkImage					image;
					m_colorSubresourceRange,									// VkImageSubresourceRange	subresourceRange;
				},
			};

			vk.cmdPipelineBarrier(*m_cmdBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0u,
				0u, DE_NULL, 0u, DE_NULL, DE_LENGTH_OF_ARRAY(barriers), barriers);
		}
		// Color image -> host buffer
		{
			const VkBufferImageCopy region =
			{
				0ull,																		// VkDeviceSize                bufferOffset;
				0u,																			// uint32_t                    bufferRowLength;
				0u,																			// uint32_t                    bufferImageHeight;
				makeImageSubresourceLayers(VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u),			// VkImageSubresourceLayers    imageSubresource;
				makeOffset3D(0, 0, 0),														// VkOffset3D                  imageOffset;
				makeExtent3D(m_renderSize.x(), m_renderSize.y(), 1u),						// VkExtent3D                  imageExtent;
			};

			vk.cmdCopyImageToBuffer(*m_cmdBuffer, *m_colorImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, colorBuffer, 1u, &region);
		}
		// Buffer write barrier
		{
			const VkBufferMemoryBarrier barriers[] =
			{
				{
					VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,		// VkStructureType    sType;
					DE_NULL,										// const void*        pNext;
					VK_ACCESS_TRANSFER_WRITE_BIT,					// VkAccessFlags      srcAccessMask;
					VK_ACCESS_HOST_READ_BIT,						// VkAccessFlags      dstAccessMask;
					VK_QUEUE_FAMILY_IGNORED,						// uint32_t           srcQueueFamilyIndex;
					VK_QUEUE_FAMILY_IGNORED,						// uint32_t           dstQueueFamilyIndex;
					colorBuffer,									// VkBuffer           buffer;
					0ull,											// VkDeviceSize       offset;
					VK_WHOLE_SIZE,									// VkDeviceSize       size;
				},
			};

			vk.cmdPipelineBarrier(*m_cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_HOST_BIT, 0u,
				0u, DE_NULL, DE_LENGTH_OF_ARRAY(barriers), barriers, DE_NULL, 0u);
		}

		VK_CHECK(vk.endCommandBuffer(*m_cmdBuffer));
		submitCommandsAndWait(vk, device, queue, *m_cmdBuffer);
	}

private:
	const UVec2								m_renderSize;
	const VkFormat							m_colorFormat;
	const VkImageSubresourceRange			m_colorSubresourceRange;
	const Vec4								m_clearColor;
	const int								m_numViewports;
	const std::vector<PositionColorVertex>	m_vertices;

	Move<VkImage>							m_colorImage;
	MovePtr<Allocation>						m_colorImageAlloc;
	Move<VkImageView>						m_colorAttachment;
	SharedPtr<Buffer>						m_vertexBuffer;
	Move<VkShaderModule>					m_vertexModule;
	Move<VkShaderModule>					m_fragmentModule;
	Move<VkRenderPass>						m_renderPass;
	Move<VkFramebuffer>						m_framebuffer;
	Move<VkPipelineLayout>					m_pipelineLayout;
	Move<VkPipeline>						m_pipeline;
	Move<VkCommandPool>						m_cmdPool;
	Move<VkCommandBuffer>					m_cmdBuffer;

	// "deleted"
				Renderer	(const Renderer&);
	Renderer&	operator=	(const Renderer&);
};

tcu::TestStatus test (Context& context, const int numViewports)
{
	const VkPhysicalDeviceFeatures&		features	= context.getDeviceFeatures();
	if (!features.multiViewport)
		TCU_THROW(NotSupportedError, "Required feature is not supported: multiViewport");

	const VkPhysicalDeviceLimits&		limits		= getPhysicalDeviceProperties(context.getInstanceInterface(), context.getPhysicalDevice()).limits;
	if (limits.maxViewports < MIN_MAX_VIEWPORTS)
		return tcu::TestStatus::fail("multiViewport supported but maxViewports is less than the minimum required");

	const std::vector<std::string>&		extensions	= context.getDeviceExtensions();
	if (!de::contains(extensions.begin(), extensions.end(), "VK_EXT_shader_viewport_index_layer"))
		TCU_THROW(NotSupportedError, "Extension VK_EXT_shader_viewport_index_layer not supported");

	const DeviceInterface&			vk					= context.getDeviceInterface();
	const VkDevice					device				= context.getDevice();
	Allocator&						allocator			= context.getDefaultAllocator();

	const UVec2						renderSize			(128, 128);
	const VkFormat					colorFormat			= VK_FORMAT_R8G8B8A8_UNORM;
	const Vec4						clearColor			(0.5f, 0.5f, 0.5f, 1.0f);
	const std::vector<Vec4>			colors				= generateColors(numViewports);
	const std::vector<UVec4>		cells				= generateGrid(numViewports, renderSize);

	const VkDeviceSize				colorBufferSize		= renderSize.x() * renderSize.y() * tcu::getPixelSize(mapVkFormat(colorFormat));

	const SharedPtr<Buffer>			colorBuffer			= Buffer::createAndAlloc(vk, device, makeBufferCreateInfo(colorBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT), allocator, MemoryRequirement::HostVisible);

	// Zero buffer.
	{
		const Allocation alloc = colorBuffer->getBoundMemory();
		deMemset(alloc.getHostPtr(), 0, static_cast<std::size_t>(colorBufferSize));
		flushMappedMemoryRange(vk, device, alloc.getMemory(), alloc.getOffset(), colorBufferSize);
	}

	{
		context.getTestContext().getLog()
			<< tcu::TestLog::Message << "Rendering a colorful grid of " << numViewports << " rectangle(s)." << tcu::TestLog::EndMessage
			<< tcu::TestLog::Message << "Not covered area will be filled with a gray color." << tcu::TestLog::EndMessage;
	}

	// Draw
	{
		const Renderer renderer (context, renderSize, numViewports, cells, colorFormat, clearColor, colors);
		renderer.draw(context, colorBuffer->object());
	}

	// Log image
	{
		const Allocation alloc = colorBuffer->getBoundMemory();
		invalidateMappedMemoryRange(vk, device, alloc.getMemory(), 0ull, colorBufferSize);

		const tcu::ConstPixelBufferAccess	resultImage		(mapVkFormat(colorFormat), renderSize.x(), renderSize.y(), 1u, alloc.getHostPtr());
		const tcu::TextureLevel				referenceImage	= generateReferenceImage(mapVkFormat(colorFormat), renderSize, clearColor, cells, colors);

		// Images should now match.
		if (!tcu::floatThresholdCompare(context.getTestContext().getLog(), "color", "Image compare", referenceImage.getAccess(), resultImage, Vec4(0.02f), tcu::COMPARE_LOG_RESULT))
			return tcu::TestStatus::fail("Rendered image is not correct");
	}

	return tcu::TestStatus::pass("OK");
}

} // anonymous

tcu::TestCaseGroup* createShaderViewportIndexTests	(tcu::TestContext& testCtx)
{
	MovePtr<tcu::TestCaseGroup> group (new tcu::TestCaseGroup(testCtx, "shader_viewport_index", ""));

	for (int numViewports = 1; numViewports <= MIN_MAX_VIEWPORTS; ++numViewports)
		addFunctionCaseWithPrograms(group.get(), "vertex_shader_" + de::toString(numViewports), "", initPrograms, test, numViewports);

	return group.release();
}

} // Draw
} // vkt
