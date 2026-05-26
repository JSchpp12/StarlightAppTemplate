#pragma once

#include <starlight/core/renderer/DefaultRenderer.hpp>

#include <vulkan/vulkan.hpp>

namespace renderer
{
struct GiveImageToTransfer
{
    void record(vk::CommandBuffer cmdBuffer);
    
    vk::ImageMemoryBarrier2 makeImgBarrier() const; 

    vk::Image image;
    vk::ImageAspectFlags aspectFlags;
    vk::ImageLayout srcImageLayout; 
    vk::ImageLayout dstImageLayout;
};
} // namespace renderer
