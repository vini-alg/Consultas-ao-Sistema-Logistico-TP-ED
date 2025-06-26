# Logistics System Query Processor

This project implements a query processing system for a logistics management system. It processes events related to package handling and provides query interfaces to retrieve package and client information.

## Project Structure

- `src/`: Contains all source code files
  - `main.cpp`: Entry point of the application
  - `logistics_system.cpp`: Implementation of the logistics system
- `include/`: Header files
  - `logistics_system.hpp`: Core data structures and interfaces
- `bin/`: Output directory for the compiled executable
- `obj/`: Directory for object files
- `Makefile`: Build configuration

## Building the Project

To build the project, run:

```bash
make
```

This will create the executable at `bin/tp3.out`.

## Running the Program

Run the program by piping the input file to the executable:

```bash
./bin/tp3.out < input.txt
```

## Input Format

The input consists of a series of events and queries, one per line, with the following formats:

1. Event:
   ```
   <timestamp> EV <package_id> <event_type> [additional_fields...]
   ```

2. Package Query:
   ```
   <timestamp> PC <package_id>
   ```

3. Client Query:
   ```
   <timestamp> CL <client_name>
   ```

## Output Format

The program will output the results of the queries in the format specified in the problem statement.

## Dependencies

- C++11 or later
- GNU Make
- GCC or compatible C++ compiler