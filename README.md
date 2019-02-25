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

Intel(R) Core(TM) i7-3770 CPU @ 3.40GHz
4 Cores/8 Threads
Hard Drive (3TB/7200rpm)
SSD (128GB)

## Results
HDD
1.3TB hipo files
hipo3test 49,252,490 Events in 13,465.422 sec
hipo3test 3,657.7 Hz
4kHz

136GB root files
rootTest 49,752,570 Event in 3,727.85 sec
rootTest 13,346.2 Hz
13kHz

SSD
80GB hipo files
hipo3test 3,041,806 Events in 644.25 sec
hipo3test 4,721.5 Hz
5kHz

8.5GB root files
rootTest 3,072,249 Event in 166.23 sec
rootTest 18,482.3 Hz
18kHz
