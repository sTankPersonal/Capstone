#pragma once
#include <cstdint>
#include <optional>
#include <vector>

template<typename TEntity, typename TId>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual std::optional<TEntity> findById(TId id) = 0;
    virtual std::vector<TEntity> findAll() = 0;
    virtual TEntity create(const TEntity& entity) = 0;
    virtual bool update(const TEntity& entity) = 0;
    virtual bool remove(TId id) = 0;
};
