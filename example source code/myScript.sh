#!/bin/bash

clear
echo -n "Enter your name: "
read name

clear

echo "Hello $name. "
echo "What is your favorite color? "
read color

echo  "$color is a good color"
echo "Now saving that info"
echo "$name favorite color is $color." >> color.log

echo "Data saved."
echo "Please Press enter to continue.."
read

clear

echo "Have a good day $name!"

