### ControllerBackup

A tool to Download and backup Falcon v4, FPP, and Genius controller JSON files.

### Building
Uses C++20, QT 5.15, spdlog, and cMake.

```git clone https://github.com/computergeek1507/ControllerBackup.git```

To build on Windows, use Visual Studio 2022

```VS2022.bat```

If you get a qt cmake error, update the QT location in batch file.

To build on Linux with g++(tested on Mint Linux 21).

```
mkdir build
cd build
cmake ..
cmake --build .
./ControllerBackup
```
