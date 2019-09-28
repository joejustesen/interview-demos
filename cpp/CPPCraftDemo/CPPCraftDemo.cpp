#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <ratio>
#include <functional>

#include "StopWatch.h"

/**
    Represents a Record Object
*/
struct QBRecord {
    uint        column0;			// unique id column
    std::string column1;
    long        column2;
    std::string column3;
};

/**
Represents a Record Collections
*/
using QBRecordCollection = std::vector<QBRecord>;

/**
    Return records that contains a string in the StringValue field
    records - the initial set of records to filter
    matchString - the string to search for
*/

// QBRecordCollection QBFindMatchingRecords (
//     const QBRecordCollection & records,
// 	const std::string & columnName,
// 	const std::string & matchString)
// {
//     QBRecordCollection result;

//     std::copy_if(records.begin(), records.end(), std::back_inserter(result), [columnName, matchString](QBRecord rec) {
		
// 		if (columnName == "column0") {
//             uint matchValue = std::stoul (matchString);
//             return matchValue == rec.column0;
// 		} else if (columnName == "column1") {
//             return rec.column1.find(matchString) != std::string::npos;
            
// 		} else if (columnName == "column2") {
//             long matchValue = std::stol (matchString);
//             return matchValue == rec.column2;
// 		} else if (columnName == "column3"){
//             return rec.column3.find (matchString) != std::string::npos;
// 		} else {
//             return false;
// 		}
//     });

//     return result;
// }



template<typename LAMBDA>
auto findByFunction(const QBRecordCollection & data, LAMBDA fn) 
{
    QBRecordCollection results;

    std::copy_if(std::begin(data), std::end(data), std::back_inserter(results), [&fn](const QBRecord & rec) {
        return fn(rec);
    });

    return results;
}

/**
    Utility to populate a record collection
    prefix - prefix for the string value for every record
    numRecords - number of records to populate in the collection
*/
QBRecordCollection populateDummyData(const std::string & prefix, uint numRecords)
{
    std::cout << "generating test data, prefix = '" << prefix << "', record count = " << numRecords << "\n";
    auto data = QBRecordCollection{};
    data.reserve(numRecords);

    for (auto i = 0u; i < numRecords; ++i) {
        data.emplace_back(QBRecord{ 
            i, 
            prefix + std::to_string(i), 
            i % 100, 
            std::to_string(i) + prefix 
        });
    }

    std::cout << "finished generating test data\n";
    return data;
}

auto findColumn0(const QBRecordCollection & data, uint value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return rec.column0 == value;
    });
}

auto findColumn1(const QBRecordCollection & data, std::string_view value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return rec.column1.find(value) != std::string::npos;
    });
}

auto findColumn2(const QBRecordCollection & data, long value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return rec.column2 == value;
    });
}

auto findColumn3(const QBRecordCollection & data, std::string_view value)
{
    return findByFunction(data, [value](const QBRecord & rec){
        return rec.column3.find(value) != std::string::npos;
    });
}

bool validateFind(const QBRecordCollection & data)
{
    {
        auto name = "find string with findColumn0";
        auto found = findColumn0(data, 3465);

        if (found.size() != 1) {
            std::cerr << "validation failed: " << name << " expected '1' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find string with findColumn1";
        auto found = findColumn1(data, "testdata500");

        if (found.size() != 111) {
            std::cerr << "validation failed: " << name << " expected '111' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find number with findColumn2";
        auto found = findColumn2(data, 24);

        if (found.size() != 1000) {
            std::cerr << "validation failed: " << name << " expected '1,000' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find string with findColumn3";
        auto found = findColumn3(data, "500testdata");

        if (found.size() != 100) {
            std::cerr << "validation failed: " << name << " expected '100' record, found '" << found.size() << "' records\n";
            return false;
        }
    }

    return true;
}


int main ()
{
    const uint RECORD_COUNT = 100'000;
    auto data = populateDummyData("testdata", RECORD_COUNT);

    if (!validateFind(data)) {
        return 1;
    }

    std::cout << "starting performance test\n";
    const int LOOP_COUNT = 1000;
    auto stopWatch = StopWatch{};

    for (auto idx = 0; idx < LOOP_COUNT; ++idx) {
        auto filteredSet = findColumn1(data, "testdata500");
        auto filteredSet2 = findColumn2(data, 24);
    }

    stopWatch.stop();
    std::cout << "finished performance test\n\n";

    std::cout << "size of record: " << sizeof(QBRecord) << " bytes\n";
    std::cout << "number of records: " << RECORD_COUNT << "\n";
    std::cout << "size of collection: " << sizeof(QBRecord) * data.size() << " bytes\n";
    std::cout << "loop counts: " << LOOP_COUNT << "\n\n";
    
    std::cout << "total duration: " << std::fixed << stopWatch.seconds() << "s\n";
    std::cout << "lookup duration: " << std::fixed << stopWatch.duration<std::chrono::microseconds>().count() / double(LOOP_COUNT) << "µs\n";
    
    return 0;
}
