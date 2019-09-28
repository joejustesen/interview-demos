#include <stdio.h>
// #include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <ratio>

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

QBRecordCollection QBFindMatchingRecords (
    const QBRecordCollection & records,
	const std::string & columnName,
	const std::string & matchString)
{
    QBRecordCollection result;

    std::copy_if(records.begin(), records.end(), std::back_inserter(result), [columnName, matchString](QBRecord rec) {
		
		if (columnName == "column0") {
            uint matchValue = std::stoul (matchString);
            return matchValue == rec.column0;
		} else if (columnName == "column1") {
            return rec.column1.find(matchString) != std::string::npos;
            
		} else if (columnName == "column2") {
            long matchValue = std::stol (matchString);
            return matchValue == rec.column2;
		} else if (columnName == "column3"){
            return rec.column3.find (matchString) != std::string::npos;
		} else {
            return false;
		}
    });

    return result;
}

/**
    Utility to populate a record collection
    prefix - prefix for the string value for every record
    numRecords - number of records to populate in the collection
*/
QBRecordCollection populateDummyData(const std::string & prefix, uint numRecords)
{
    QBRecordCollection data;
    
    data.reserve(numRecords);

    for (auto i = 0u; i < numRecords; ++i) {
        QBRecord rec = { i, prefix + std::to_string (i), i % 100,
        std::to_string (i) + prefix };
        
        data.emplace_back(rec);
    }

    return data;
}

bool validateFind(const QBRecordCollection & data)
{
    {
        auto name = "find string in column1";
        auto found = QBFindMatchingRecords(data, "column1", "testdata500");
        if (found.size() != 1) {
            std::cerr << "validation failed: " << name << " expected '1' record, found '" << found.size() << "' records\n";
            return false;
        }
    }
    {
        auto name = "find number in column2";
        auto found = QBFindMatchingRecords(data, "column2", "24");
        if (found.size() != 10) {
            std::cerr << "validation failed: " << name << " expected '10' record, found '" << found.size() << "' records\n";
            return false;
        }
    }

    return true;
}


int main ()
{
    const uint RECORD_COUNT = 1000;
    auto data = populateDummyData("testdata", RECORD_COUNT);

    if (!validateFind(data)) {
        return 1;
    }

    const int LOOP_COUNT = 1000;
    auto stopWatch = StopWatch{};

    for (auto idx = 0; idx < LOOP_COUNT; ++idx) {
        auto filteredSet = QBFindMatchingRecords(data, "column1", "testdata500");
        auto filteredSet2 = QBFindMatchingRecords(data, "column2", "24");
    }

    std::cout << "size of record: " << sizeof(QBRecord) << " bytes\n";
    std::cout << "number of records: " << RECORD_COUNT << "\n";
    std::cout << "size of collection: " << sizeof(QBRecord) * data.size() << " bytes\n";
    std::cout << "loop counts: " << LOOP_COUNT << "\n\n";
    
    std::cout << "total duration: " << std::fixed << stopWatch.seconds() << "s\n";
    std::cout << "lookup duration: " << std::fixed << stopWatch.duration<std::chrono::microseconds>().count() / double(LOOP_COUNT) << "µs\n";
    
    return 0;
}
