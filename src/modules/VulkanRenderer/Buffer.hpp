//
// Created by Matthew McCall on 5/13/22.
//

#ifndef YORK_VULKAN_BUFFER_HPP
#define YORK_VULKAN_BUFFER_HPP

#include <algorithm>

#include <vulkan/vulkan.hpp>

#include "Device.hpp"
#include "Instance.hpp"
#include "vk_mem_alloc.h"

namespace Si::Vulkan {

class Buffer : public Handle<vk::Buffer>
{
public:
    Buffer(Instance &instance, Device &device, std::size_t size);

    template <typename T>
    void copyData(Vector<T> buffer)
    {
        assert(isCreated());
        std::memcpy(m_allocationInfo.pMappedData, buffer.data(), std::min(m_size, sizeof(buffer.front()) * buffer.size()));
    }

private:
    bool createImpl() override;
    void destroyImpl() override;

    Instance &m_instance;
    Device &m_device;

    VmaAllocator m_allocator = nullptr;
    VmaAllocation m_allocation = nullptr;
    VmaAllocationInfo m_allocationInfo;

    std::size_t m_size;
};

}

#endif // YORK_VULKAN_BUFFER_HPP
