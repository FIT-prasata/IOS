# IOS
My solution to Operating Systems class projects at VUT FIT (second semester)

## Project 1

Shell script for analyzing COVID-19 cases in Czech republic

### Usage

General usage

```bash
corona [-h] [FILTERS] [COMMAND] [LOG [LOG2 [...]]
```

and

```bash
./corona -h
```

to display help where the use cases are precisely described

### File Handle

#### Valid file header format

```bash
id,datum,vek,pohlavi,kraj_nuts_kod,okres_lau_kod,nakaza_v_zahranici,nakaza_zeme_csu_kod,reportovano_khs
```

#### Testing files

osoby-short.csv - shorter version of the offical 250MB+ osoby.csv file (https://onemocneni-aktualne.mzcr.cz/api/v2/covid-19/osoby.csv) BE CAREFUL!

osoby2.csv - data with blank lines, whitespaces and invalid columns for testing purposes, script will format and handle those as well

osoby-short.csv.gz - gzip compressed version of osoby-short.csv

osoby-short.csv.bz2 - bzip2 compressed version of osoby-short.csv

infected-jan22 - directort full of *.csv files to easily demonstrate working multiple files input 

#### Multiple files

Script handles multiple files on input by merging them into one big file and removing all the headers except for the one in first file

Example:

```bash
./corona infected infected-jan22/2022-01-*.csv
560894
```

#### Empty file

Script handles empty file by displaying only its header

Example:

```bash
cat /dev/null | ./corona
id,datum,vek,pohlavi,kraj_nuts_kod,okres_lau_kod,nakaza_v_zahranici,nakaza_zeme_csu_kod,reportovano_khs
```

#### No file

Script handles this scenario by taking the data from STDIN

Example for first 5 lines of osoby-short.csv:

```bash
cat osoby-short.csv | head -n 5 | ./corona
id,datum,vek,pohlavi,kraj_nuts_kod,okres_lau_kod,nakaza_v_zahranici,nakaza_zeme_csu_kod,reportovano_khs
6f4125cb-fb41-4fb0-a478-07b69ba106a4,2020-03-01,21,Z,CZ010,CZ0100,1,IT,1
5841443b-7df4-4af9-acab-75ca47010ec3,2020-03-01,43,M,CZ042,CZ0421,1,IT,1
5cdb7ece-97a2-4336-9715-59dc70a48a2c,2020-03-01,67,M,CZ010,CZ0100,1,IT,1
d345e0e2-9056-4d3f-b790-485b12831180,2020-03-03,21,Z,CZ010,CZ0100,,,
```

#### Compressed file handle

Script can decompress .gz and .bz2 files automatically

### Validation

Script validates flags, commands and file suffixes given by user and their values, also checks for valid date and age inside the file

Empty columns will be aggregated to None on output except for countries command

### Windows new line problem fix

Windows sometimes messes up the script by adding a carriage return character after \n at the end of lines

This command should fix the issue by replacing the \r character by an empty string

```bash
sed -i -e 's/\r$//' corona
```

### TODO

Extend regions and districts commands to display location names instead of their codes
