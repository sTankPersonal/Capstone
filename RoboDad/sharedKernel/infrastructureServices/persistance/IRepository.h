#pragma once
#include <cstdint>
#include <optional>

template<typename TEntity, typename TId = uint32_t>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual std::optional<TEntity> findById(TId id) = 0;
    virtual bool update(const TEntity& entity) = 0;
    virtual bool remove(TId id) = 0;
};
