#!/usr/bin/env bash

#/**********************************************
# * Last Name:   CHEN
# * First Name:  BENKUN
# * Student ID:  30005337
# * Course:      CPSC 457
# * Tutorial:    T02
# * Assignment:  2
# * Question:    Q5
# *
# * File name: scan.sh
# *********************************************/

# The program takes the arguments like "./scan.sh jpg 5"
# $0 = ./scan.sh
# $1 = jpg
# $2 = 5

# find <params> | sort <params> | head <params> | awk <params>
find . -type f -name "*$1" -printf '%p %s\n' | sort -t " " -k 2 -n -r | head -$2 | awk '{ x += $2 ; print $1,$2 } END { print "Total size: "x }'
