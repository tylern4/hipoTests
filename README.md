# hipoTests

## Build

To build, must have a version of coatjava and environment variable CLAS12LIBS set to the path of coatjava.

```bash
export CLAS12LIBS=/path/to/dir/containing/coatjava
mkdir build && cd build
cmake ..
make
```

This will build the jar file for hipo3test/hipo4test as well as the rootTest.

## Getting the root files
Files were converted with dst2root_3 which can be found and built using the [hipo_tools](https://github.com/JeffersonLab/hipo_tools) package.

They can also be converted with:

```bash
docker run --rm -it -v $PWD:/data tylern4/hipo_tools dst2root_3 /data/file.hipo
```

Where file.hipo is in the current folder.
Make sure to append '/data/' to the file so that docker can find the file and convert it.

## Running Tests

To run java tests:
```bash
java -cp "hipo3test.jar:$CLAS12LIBS/*" hipo3test "/path/to/hipo3/files/*.hipo" beam_energy
```

```bash
./rootTest "/path/to/dst2root/files/*.root" beam_energy
```

## Test setup

| Computer/software for testing           	|
|-----------------------------------------	|
| Ubuntu 17.10                            	|
| openjdk version "1.8.0_171"             	|
| coat-libs-5.7.7-SNAPSHOT.jar            	|
| ROOT 6.17/01                            	|
| gcc (Ubuntu 7.2.0-8ubuntu3.2) 7.2.0     	|
| Intel(R) Core(TM) i7-3770 CPU @ 3.40GHz 	|
| 4 Cores/8 Threads                       	|
| HDD (3TB/7200rpm)                       	|
| SSD (128GB)                             	|

## Results

Files originally from /work/clas12/clas12/data/calib/cooked_5p7p4_fullMap
All files from run 5700 copied to local computer for testing.


All files, on HDD:

|    Test   	| File Size 	|  Events processed 	|   Time (sec)   	| Events/Sec (Hz) 	|   kHz   	|
|:---------:	|:---------:	|:-----------------:	|:--------------:	|:---------------:	|:-------:	|
| hipo3test 	|   1.3TB   	| 49,252,490 Events 	| 13,465.422 Sec 	|    3,657.7 Hz   	|  ~ 4kHz 	|
|  rootTest 	|   136GB   	| 49,752,570 Events 	|  3,727.85 sec  	|   13,346.2 Hz   	| ~ 13kHz 	|


Selection of 50 files, on SSD:

|    Test   	| File Size 	| Events processed 	| Time (sec) 	| Events/Sec (Hz) 	|   kHz   	|
|:---------:	|:---------:	|:----------------:	|:----------:	|:---------------:	|:-------:	|
| hipo3test 	|    80GB   	| 3,041,806 Events 	| 644.25 Sec 	|    4,721.5 Hz   	|  ~ 5kHz 	|
|  rootTest 	|   8.5GB   	| 3,072,249 Events 	| 166.23 sec 	|   18,482.3 Hz   	| ~ 18kHz 	|
