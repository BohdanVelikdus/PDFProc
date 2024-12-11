PDF to SQLite Project

<br>

Overview

This project is a streamlined solution for reading data from PDF files and storing it into a SQLite database. 
Key Features

PDF Parsing: Utilizes the Poppler library to extract text and metadata from PDF files.

Database Operations: Employs sqlite-modern-cpp for intuitive SQLite database operations.

Cross-Platform Support: Build and run on multiple platforms using CMake and vcpkg.

Requirements

Libraries:

```Poppler```

```sqlite-modern-cpp```

Dependency Manager:

```vcpkg```

Build System:

```CMake```

Test Environment:

Docker image for SQLite database (available in SQLiteContainer folder).

Setup Guide

Install vcpkg

Clone the vcpkg repository:

```git clone https://github.com/microsoft/vcpkg.git```

Navigate to the vcpkg directory and bootstrap:

```
cd vcpkg
./bootstrap-vcpkg.sh 
```

Configure CMakeUserPresets.json

Create a CMakeUserPresets.json file in the root of your project directory with the following structure:
```
{
    "version": 2,
    "configurePresets": 
    [
        {
            "name": "default-debug",
            "inherits": "vcpkg-default",
            "environment": 
            {
                "VCPKG_ROOT": "/path/to/vcpkg",
                "CXX_COMPILER": "/path/to/g++",
                "C_COMPILER": "/path/to/gcc"
            }
        }
    ]
}
```

Building the Project

Create a build directory:

```mkdir build && cd build```

Configure the project:

```cmake .. --preset=default-debug```

Build the project:

```cmake --build . ```

Testing with Docker

Navigate to the SQLiteContainer folder:

```cd SQLiteContainer```

Build the Docker image:

```docker compose up -d```

When you can access the container using: 
```docker exec -it <docker id> /bin/sh```
