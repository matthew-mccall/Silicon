//
// Created by Matthew McCall on 5/13/22.
//

#include <utility>

#include "Silicon/Types.hpp"
#include "boost/uuid/uuid_hash.hpp"

#include "Buffer.hpp"

using AllocatorMapKey = std::pair<Si::NotNull<Si::Vulkan::Instance *>, Si::NotNull<Si::Vulkan::Device *>>;

namespace {

Si::Map<AllocatorMapKey, unsigned> s_referenceCount;
Si::Map<AllocatorMapKey, VmaAllocator> s_allocators;

}

namespace Si::Vulkan {

Buffer::Buffer(Instance &instance, Device &device, std::size_t size)
    : m_instance(instance)
    , m_device(device)
    , m_size(size)
{
    addDependency(m_instance);
    addDependency(m_device);

    if (s_allocators.find({NotNull<Instance *>(&instance), NotNull<Device *>(&device)}) == s_allocators.end()) {

        assert(!(s_referenceCount[{NotNull<Instance *>(&instance), NotNull<Device *>(&device)}]));

        VmaAllocatorCreateInfo createInfo {};

        createInfo.vulkanApiVersion = VK_API_VERSION_1_1;
        createInfo.physicalDevice = *m_device.getPhysicalDevice();
        createInfo.device = *m_device;
        createInfo.instance = *m_instance;

        vmaCreateAllocator(&createInfo, &s_allocators[{NotNull<Instance *>(&instance), NotNull<Device *>(&device)}]);
    }

    m_allocator = s_allocators[{NotNull<Instance *>(&instance), NotNull<Device *>(&device)}];
    s_referenceCount[{NotNull<Instance *>(&instance), NotNull<Device *>(&device)}]++;
}

bool Buffer::createImpl()
{
    vk::BufferCreateInfo createInfo {{}, m_size, vk::BufferUsageFlagBits::eVertexBuffer, vk::SharingMode::eExclusive};

    VmaAllocationCreateInfo allocationCreateInfo {
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        VMA_MEMORY_USAGE_AUTO,
        static_cast<VkMemoryPropertyFlags>(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)};

    vmaCreateBuffer(
        m_allocator,
        reinterpret_cast<const VkBufferCreateInfo *>(&createInfo),
        &allocationCreateInfo,
        reinterpret_cast<VkBuffer *>(&m_handle),
        &m_allocation,
        &m_allocationInfo);

    return true;
}

void Buffer::destroyImpl()
{
    vmaDestroyBuffer(m_allocator, m_handle, m_allocation);
    s_referenceCount[{NotNull<Instance *>(&m_instance), NotNull<Device *>(&m_device)}]--;

    if (!s_referenceCount[{NotNull<Instance *>(&m_instance), NotNull<Device *>(&m_device)}]) {
        vmaDestroyAllocator(m_allocator);
    }
}

}
