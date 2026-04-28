#!/bin/bash


#single opch extraction-----------------------
#Pay attention: location of file must be at ./root_data/
inputFile="corsika_rsl150_abs20_3_hist"
outputDir="/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/new_20250127/rsl_compare_result/results/tmp/"

#single opch combination----------------------
input_path=$outputDir
input_name="_${inputFile}.root"
output_path=$input_path
#output_path="/Users/shuaixiangzhang/Work/current/FNAL_Work2024/rsl_analyses/v4_analysis/results/rsl99_1000num2_e67_crtCut/"



#===Extract Single opch information==========================
#Content of M1, opch 0, 2, 17, 23---
M1="./src/singleOpChM1.cxx"
opch00=0
opch00s="00"
opch02=2
opch02s="02"
opch17=17
opch17s="17"
opch23=23
opch23s="23"

#Content of M2, opch 1, 3, 16, 22---
M2="./src/singleOpChM2.cxx"
opch01=1
opch01s="01"
opch03=3
opch03s="03"
opch16=16
opch16s="16"
opch22=22
opch22s="22"

#Content ofC1, opch 4~11---
C1="./src/singleOpChC1.cxx"
opch04=4
opch04s="04"
opch05=5
opch05s="05"
opch06=6
opch06s="06"
opch07=7
opch07s="07"
opch08=8
opch08s="08"
opch09=9
opch09s="09"
opch10=10
opch10s="10"
opch11=11
opch11s="11"

#Content of P1, opch 31, 33---
P1="./src/singleOpChP1.cxx"
opch31=31
opch31s="31"
opch33=33
opch33s="33"

#Content of P2, opch 12~15, 18~21, 24~29, 30, 32, 34~39---
P2="./src/singleOpChP2.cxx"
opch12=12
opch12s="12"
opch13=13
opch13s="13"
opch14=14
opch14s="14"
opch15=15
opch15s="15"
opch18=18
opch18s="18"
opch19=19
opch19s="19"
opch20=20
opch20s="20"
opch21=21
opch21s="21"
opch24=24
opch24s="24"
opch25=25
opch25s="25"
opch26=26
opch26s="26"
opch27=27
opch27s="27"
opch28=28
opch28s="28"
opch29=29
opch29s="29"
opch30=30
opch30s="30"
opch32=32
opch32s="32"
opch34=34
opch34s="34"
opch35=35
opch35s="35"
opch36=36
opch36s="36"
opch37=37
opch37s="37"
opch38=38
opch38s="38"
opch39=39
opch39s="39"


#Create a temporary ROOT script to execute the main and exit---
#For M1-----------------------------------------------------
temp_scriptM1=$(mktemp)
echo ".x $M1(\"$inputFile\", \"$outputDir\", $opch00, \"$opch00s\")" > "$temp_scriptM1"
echo ".x $M1(\"$inputFile\", \"$outputDir\", $opch02, \"$opch02s\")" >> "$temp_scriptM1"
echo ".x $M1(\"$inputFile\", \"$outputDir\", $opch17, \"$opch17s\")" >> "$temp_scriptM1"
echo ".x $M1(\"$inputFile\", \"$outputDir\", $opch23, \"$opch23s\")" >> "$temp_scriptM1"

echo ".q" >> "$temp_scriptM1"
#Run the temporary ROOT script
root -l -b -q "$temp_scriptM1"
#Remove the temporary script
rm "$temp_scriptM1"


#For M2----------------------------------------------------
temp_scriptM2=$(mktemp)
echo ".x $M2(\"$inputFile\", \"$outputDir\", $opch01, \"$opch01s\")" > "$temp_scriptM2"
echo ".x $M2(\"$inputFile\", \"$outputDir\", $opch03, \"$opch03s\")" >> "$temp_scriptM2"
echo ".x $M2(\"$inputFile\", \"$outputDir\", $opch16, \"$opch16s\")" >> "$temp_scriptM2"
echo ".x $M2(\"$inputFile\", \"$outputDir\", $opch22, \"$opch22s\")" >> "$temp_scriptM2"

echo ".q" >> "$temp_scriptM2"
#Run the temporary ROOT script
root -l -b -q "$temp_scriptM2"
#Remove the temporary script
rm "$temp_scriptM2"


#For C1----------------------------------------------------
temp_scriptC1=$(mktemp)
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch04, \"$opch04s\")" > "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch05, \"$opch05s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch06, \"$opch06s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch07, \"$opch07s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch08, \"$opch08s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch09, \"$opch09s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch10, \"$opch10s\")" >> "$temp_scriptC1"
echo ".x $C1(\"$inputFile\", \"$outputDir\", $opch11, \"$opch11s\")" >> "$temp_scriptC1"

echo ".q" >> "$temp_scriptC1"
#Run the temporary ROOT script
root -l -b -q "$temp_scriptC1"
#Remove the temporary script
rm "$temp_scriptC1"


#For P1----------------------------------------------------
temp_scriptP1=$(mktemp)
echo ".x $P1(\"$inputFile\", \"$outputDir\", $opch31, \"$opch31s\")" > "$temp_scriptP1"
echo ".x $P1(\"$inputFile\", \"$outputDir\", $opch33, \"$opch33s\")" >> "$temp_scriptP1"

echo ".q" >> "$temp_scriptP1"
#Run the temporary ROOT script
root -l -b -q "$temp_scriptP1"
#Remove the temporary script
rm "$temp_scriptP1"

#For P2----------------------------------------------------
temp_scriptP2=$(mktemp)
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch12, \"$opch12s\")" > "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch13, \"$opch13s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch14, \"$opch14s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch15, \"$opch15s\")" >> "$temp_scriptP2"

echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch18, \"$opch18s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch19, \"$opch19s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch20, \"$opch20s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch21, \"$opch21s\")" >> "$temp_scriptP2"

echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch24, \"$opch24s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch25, \"$opch25s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch26, \"$opch26s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch27, \"$opch27s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch28, \"$opch28s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch29, \"$opch29s\")" >> "$temp_scriptP2"

echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch30, \"$opch30s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch32, \"$opch32s\")" >> "$temp_scriptP2"

echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch34, \"$opch34s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch35, \"$opch35s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch36, \"$opch36s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch37, \"$opch37s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch38, \"$opch38s\")" >> "$temp_scriptP2"
echo ".x $P2(\"$inputFile\", \"$outputDir\", $opch39, \"$opch39s\")" >> "$temp_scriptP2"

echo ".q" >> "$temp_scriptP2"
#Run the temporary ROOT script
root -l -b -q "$temp_scriptP2"
#Remove the temporary script
rm "$temp_scriptP2"

#============================================================




#Combine different opch======================================
combineCode="./src/opCh_combine.cxx"
temp_combine=$(mktemp)
echo ".x $combineCode(\"$input_path\", \"$input_name\", \"$output_path\")" > "$temp_combine"


echo ".q" >> "$temp_combine"
#Run the temporary ROOT script
root -l -b -q "$temp_combine"
#Remove the temporary script
rm "$temp_combine"






