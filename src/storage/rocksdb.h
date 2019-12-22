#pragma once

#include <memory>
#include "../IStorage.h"
#include <rocksdb/db.h>

class RocksDBStorageProvider : public IStorage
{
    std::shared_ptr<rocksdb::DB> m_spdb;    // Note: This must be first so it is deleted last
    std::unique_ptr<rocksdb::WriteBatch> m_spbatch;
    const rocksdb::Snapshot *m_psnapshot = nullptr;
    std::shared_ptr<rocksdb::ColumnFamilyHandle> m_spcolfamily;
    rocksdb::ReadOptions m_readOptionsTemplate;

public:
    RocksDBStorageProvider(std::shared_ptr<rocksdb::DB> &spdb, std::shared_ptr<rocksdb::ColumnFamilyHandle> &spcolfam, const rocksdb::Snapshot *psnapshot);
    ~RocksDBStorageProvider();

    virtual void insert(const char *key, size_t cchKey, void *data, size_t cb) override;
    virtual bool erase(const char *key, size_t cchKey) override;
    virtual void retrieve(const char *key, size_t cchKey, callbackSingle fn) const override;
    virtual size_t clear() override;
    virtual bool enumerate(callback fn) const override;

    virtual const IStorage *clone() const override;

    virtual void beginWriteBatch() override;
    virtual void endWriteBatch() override;

    virtual void flush() override;

    size_t count(bool fStrict) const;

protected:

    const rocksdb::ReadOptions &ReadOptions() const { return m_readOptionsTemplate; }
    rocksdb::WriteOptions WriteOptions() const;
};