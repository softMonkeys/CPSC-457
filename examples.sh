#!/usr/bin/env bash
# My first script *************************
echo "Hello world"
name="Ben"
age=21
echo $name
echo $age

# My second script *************************
name[0]="I"
name[1]="am"
name[2]="a"
name[3]="pig"

echo ${name[*]}
echo "${name[0]} ${name[1]}"
echo

# My third script *************************
number=(1 2 3 4 5)

echo ${number[*]}
echo ${number[@]}

# My fourth script *************************
num1=3
num2=5

expr $num1 + $num2
expr $num1 - $num2

let total=$num1*$num2
total2=$(($num1%$num2))
echo $total
echo $total2

# My fivth script *************************
name="Emma"
if [ "$name" = "Emma" ];then
  echo "Emanuel"
elif [ "$name" = "Peter" ];then
  echo "Peterson"
else
  echo "Unknown name"
fi
