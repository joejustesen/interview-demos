#include "stdafx.h"
#include <vector>
#include <chrono>
#include <ratio>
#include <functional>
#include <unordered_map>

#include "StopWatch.h"
#include "InMemoryIndex.h"


struct QBRecord {
    unsigned int    d_column0;			// unique id column
    std::string     d_column1;
    long            d_column2;
    std::string     d_column3;
    bool            d_deleted;
};

bool operator<(const QBRecord & lhs, const QBRecord & rhs) { 
    return lhs.d_column0 < rhs.d_column0; 
}

using QBRecordCollection = std::vector<QBRecord>;
using StringIndex = std::unordered_map<std::string, int>;


template<typename LAMBDA>
auto findByFunction(const QBRecordCollection & data, LAMBDA fn) 
{
    QBRecordCollection results;

    std::copy_if(std::begin(data), std::end(data), std::back_inserter(results), [&fn](const QBRecord & rec) {
        return fn(rec);
    });

    return results;
}

/****************************************************************************
    Do a binary search on the sorted records by ID.
****************************************************************************/
auto fastFindById(const QBRecordCollection & data, unsigned int id)
{
    auto value = QBRecord{id, "", 0, "", false };
    auto it = std::lower_bound(std::begin(data), std::end(data), value);

    if (it != std::end(data) && it->d_column0 == id) {
        return it;
    } else {
        return std::end(data);
    }
}

auto fastFindById(QBRecordCollection & data, unsigned int id)
{
    auto it = fastFindById(static_cast<const QBRecordCollection &>(data), id);
    return data.erase(it, it);  // null erase to convert const_iterator to an iterator
}


/****************************************************************************
 *  Delete record by index id.
 *  Possible bad assumption here, id may not match 
****************************************************************************/
bool deleteRecordByID(QBRecordCollection& data, unsigned int id)
{
    auto it = fastFindById(data, id);

    if (it != std::end(data)) {
        it->d_deleted = true;
        return true;
    };

    return false;
}

/****************************************************************************
 *  Generate an index for d_column1
****************************************************************************/
InMemoryIndex createIndex(const QBRecordCollection & data)
{
    auto index = InMemoryIndex{};

    for (const auto & record : data) {
        index.insert(record.d_column1.c_str(), record.d_column0);
    }

    return index;
}


/****************************************************************************
 * Create test data
 *  prefix: prefix for the string value for every record
 *  numRecords: number of records to populate in the collection
****************************************************************************/
QBRecordCollection populateDummyData(const std::string & prefix, unsigned int numRecords)
{
    std::cout << "generating test data, prefix = '" << prefix << "', record count = " << numRecords << "\n";
    auto data = QBRecordCollection{};
    data.reserve(numRecords);

    for (auto i = 0u; i < numRecords; ++i) {
        auto record = QBRecord{ 
            i, 
            prefix + std::to_string(i), 
            i % 100, 
            std::to_string(i) + prefix,
            false
        };

        data.emplace_back(record);
    }

    std::cout << "finished generating test data\n";
    return data;
}


/****************************************************************************
****************************************************************************/
auto indicesToRecords(const QBRecordCollection & data, const Indices & indices)
{
    QBRecordCollection results;

    std::transform(std::begin(indices), std::end(indices), std::back_inserter(results), 
        [&data](auto index) {
            return data[index];
        });

    results.erase(std::remove_if(std::begin(results), std::end(results), 
        [](const auto& rec) {
            return rec.d_deleted;
        }), 
        std::end(results));

    return results;
}

/****************************************************************************
****************************************************************************/
auto findColumn0(const QBRecordCollection & data, unsigned int value)
{
    auto results = QBRecordCollection{};
    auto it = fastFindById(data, value);

    if (it != std::end(data) && !data[value].d_deleted) {
        results.push_back(*it);
    }

    return results;
}


/****************************************************************************
****************************************************************************/
auto findColumn1(const QBRecordCollection & data, std::string_view value, const InMemoryIndex * index = nullptr)
{
    if (index) {
        auto [found, indices] = index->search(value);
        if (!found) {
            return QBRecordCollection{};
        } else {
            return indicesToRecords(data, indices);
        }

    } else {
        return findByFunction(data, [value](const QBRecord & rec){
            return !rec.d_deleted && rec.d_column1.find(value) != std::string::npos;
        });
    }
}


/****************************************************************************
****************************************************************************/
auto findColumn2(const QBRecordCollection & data, long value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return !rec.d_deleted && rec.d_column2 == value;
    });
}


/****************************************************************************
****************************************************************************/
auto findColumn3(const QBRecordCollection & data, std::string_view value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return !rec.d_deleted && rec.d_column3.find(value) != std::string::npos;
    });
}


/****************************************************************************
****************************************************************************/
bool validateFind()
{
    std::cout << "validating findRecord function\n";

    auto data = populateDummyData("testdata", 1000);
    std::sort(std::begin(data), std::end(data));
    const auto index = createIndex(data);
    
    {
        auto name = "find string with findColumn0";
        auto found = findColumn0(data, 346);

        if (found.size() != 1) {
            std::cerr << "validation failed 0: " << name << " expected '1' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find string with findColumn1";
        auto found = findColumn1(data, "testdata50", &index);

        if (found.size() != 11) {
            std::cerr << "validation failed 1: " << name << " expected '11' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find number with findColumn2";
        auto found = findColumn2(data, 24);

        if (found.size() != 10) {
            std::cerr << "validation failed 2: " << name << " expected '10' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find string with findColumn3";
        auto found = findColumn3(data, "500testdata");

        if (found.size() != 1) {
            std::cerr << "validation failed 3: " << name << " expected '1' record, found '" << found.size() << "' records\n";
            return false;
        }
    }

    {
        deleteRecordByID(data, 50);

        auto name = "find string with findColumn1";
        auto found = findColumn1(data, "testdata50", &index);

        if (found.size() != 10) {
            std::cerr << "validation failed 4: " << name << " expected '10' record, found '" << found.size() << "' records\n";
            return false;
        }
    }

    return true;
}


/****************************************************************************
****************************************************************************/
int main()
{
    if (!validateTrie() || !validateFind()) {
        return 1;
    }

    const unsigned int RECORD_COUNT = 100'000;
    auto data = populateDummyData("testdata", RECORD_COUNT);
    std::sort(std::begin(data), std::end(data));
    const auto index = createIndex(data);

    std::cout << "starting performance test\n";
    const int LOOP_COUNT = 1000;
    auto stopWatch = StopWatch{};

    for (auto idx = 0; idx < LOOP_COUNT; ++idx) {
        auto filteredSet = findColumn1(data, "testdata500", &index);
        auto filteredSet2 = findColumn2(data, 24);
    }

    stopWatch.stop();
    std::cout << "finished performance test\n\n";

    std::cout << "size of record: " << sizeof(QBRecord) << " bytes\n";
    std::cout << "number of records: " << RECORD_COUNT << "\n";
    std::cout << "size of collection: " << sizeof(QBRecord) * data.size() << " bytes\n";
    std::cout << "loop counts: " << LOOP_COUNT << "\n\n";
    
    std::cout << "total duration: " << std::fixed << stopWatch.seconds() << "s\n";
    std::cout << "lookup duration: " << std::fixed << double(stopWatch.duration<std::chrono::microseconds>().count()) / double(LOOP_COUNT) << "µs\n";
    
    return 0;
}
