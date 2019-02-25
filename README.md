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


| Conversion test 	| Time     	| Events/Sec 	| kHz      	|
|-----------------	|----------	|------------	|----------	|
| HDD (hipo3)     	| 75.2 Sec 	| 1390.5 Hz  	| ~ 1.4kHz 	|
| SSD (hipo3)     	| 54.4 Sec 	| 1922.9 Hz  	| ~ 2kHz   	|

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

## hipo3 Results

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

## hipo4 Results

Currently inconclusive since there is only one file to test.

```
/work/clas12/jnp/clas_004152.recon.hipo
```

The file tested is also strange hipo in java says 1,253,842 events while the max events processed is 255,341 which is only 20% of the file. An event is only counted if the first particle is negative and therefore goes through some processing/histogram filling, the programs do not count events which are skipped.

The files were converted using the same program but with different options. The first option saves all events in the file, the second option only saves events where the length of the reconstructed events is not 0, while the third option only saves events with a good identified electron as the first particle.

| Conversion test           	| Time      	| Events/Sec 	| kHz     	|
|---------------------------	|-----------	|------------	|---------	|
| SSD (hipo4) (all events)  	| 584.2 Sec 	| 2146.4 Hz  	| ~ 2.1kHz  |
| SSD (hipo4) (rec events)  	| 605.9 Sec 	| 2069.3 Hz  	| ~ 2kHz  	|
| SSD (hipo4) (elec events) 	| 65.7 Sec  	| 19085.7 Hz 	| ~ 19kHz 	|

Since the root file format is the same in both hipo3/hipo4 conversion the Events/Sec should be close to the same when reading a converted hipo3/hipo4 file however this is not the case. It would be better comparison to look at hipo3/hipo4 files for the same run but I only have one file availible for hipo4 testing and cannot find the hipo3 files for the same run.

|          Test          	| File Size 	| Events Processed 	| Time (sec) 	| Events/Sec (Hz) 	|  kHz  	|
|:----------------------:	|:---------:	|:----------------:	|:----------:	|:---------------:	|:-----:	|
|        hipo4Test       	|   2.2GB   	|      255,341     	|   7.9 Sec  	|    32,305 Hz     	| ~ 32kHz |
|  rootTest (all events) 	|   2.3GB   	|      255,341     	|  32.1 Sec  	|    7952.3 Hz    	| ~ 8kHz  |
|  rootTest (rec events) 	|   2.3GB   	|      255,341     	|  30.8 Sec  	|    8274.3 Hz    	| ~ 8kHz	|
| rootTest (elec events) 	|   204MB   	|      111,543     	|  2.85 Sec  	|    39,121 Hz    	| ~ 39kHz	|
