## Ideas

- Use google benchmark to for future testing
- change from array of structures to struture of arrays
- validate data access patterns, do we need to support partial string matches?
- changes so far have been algorithmic, not focusing on implemenation details

## Statistics

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
