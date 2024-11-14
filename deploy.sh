#!/bin/bash
set -e  # Завершается с ошибкой при любой неудачной команде
set -x  # включение отладки

ls
pwd

scp -P 699 ./C3_SimpleBashUtils-3-develop/src/cat/s21_cat q@192.168.100.10:/usr/local/bin
scp -P 699 ./C3_SimpleBashUtils-3-develop/src/grep/s21_grep q@192.168.100.10:/usr/local/bin

ssh -p 699 q@192.168.100.10 ls -lh /usr/local/bin
