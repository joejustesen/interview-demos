## Ideas

- Use google benchmark to for future testing
- change from array of structures to struture of arrays
- validate data access patterns, do we need to support partial string matches?
- changes so far have been algorithmic, not focusing on implemenation details

## Statistics

All statistics generated on Linux Ubuntu 19.04
AMD 2400G with 32GB RAM

### Starting statistics

- size of record: 80 bytes
- number of records: 100000
- size of collection: 8000000 bytes
- loop counts: 1000
- total duration: 8.454576s
- lookup duration: 8454.576000µs

### Seperate functions per lookup

- size of record: 80 bytes
- number of records: 100000
- size of collection: 8000000 bytes
- loop counts: 1000
- total duration: 1.990278s
- lookup duration: 1990.278000µs

### Indexed statistics

- size of record: 80 bytes
- number of records: 100000
- size of collection: 8000000 bytes
- loop counts: 1000
- total duration: 0.522828s
- lookup duration: 522.828000µs

### Indexed statistics with Delete

- size of record: 88 bytes
- number of records: 100000
- size of collection: 8800000 bytes
- loop counts: 1000
- total duration: 0.376851s
- lookup duration: 376.851000µs

### Fix string buffer records

- size of record: 56 bytes
- number of records: 100000
- size of collection: 5600000 bytes
- loop counts: 1000
- total duration: 0.376851s
- lookup duration: 376.851000µs
