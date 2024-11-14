CC =gcc
CFLAGS =-Wall -Werror -Wextra
OBJECT =s21_grep.c

all: s21_grep

s21_grep: clean s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) $(OBJECT) s21_grep.h -std=c11 -o s21_grep

test_grep:
	bash grep_test_with_e.sh
	# Тестируем через diff:
	# 1 файл
	grep -e world 1.txt > tmp1.txt && ./s21_grep -e world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -i world 1.txt > tmp1.txt && ./s21_grep -i world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -v world 1.txt > tmp1.txt && ./s21_grep -v world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -c world 1.txt > tmp1.txt && ./s21_grep -c world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -l world 1.txt > tmp1.txt && ./s21_grep -l world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -n world 1.txt > tmp1.txt && ./s21_grep -n world 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	#  2 файла
	grep -e world 1.txt 2.txt > tmp1.txt && ./s21_grep -e world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -i world 1.txt 2.txt > tmp1.txt && ./s21_grep -i world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -v world 1.txt 2.txt > tmp1.txt && ./s21_grep -v world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -c world 1.txt 2.txt > tmp1.txt && ./s21_grep -c world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -l world 1.txt 2.txt > tmp1.txt && ./s21_grep -l world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -n world 1.txt 2.txt > tmp1.txt && ./s21_grep -n world 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	#Проверка на смешанные флаги
	grep -vni asd 1.txt 2.txt > tmp1.txt && ./s21_grep -vni asd 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -vn asd 1.txt 2.txt > tmp1.txt && ./s21_grep -vn asd 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -iv asd 1.txt 2.txt > tmp1.txt && ./s21_grep -iv asd 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	# Проверка на -f file
	grep -f find.txt 3.txt 2.txt > tmp1.txt && ./s21_grep -f find.txt 3.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	# Проверка на  -o
	grep -o "[a-d]" 2.txt > tmp1.txt && ./s21_grep -o "[a-d]" 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -o "[a-d]" 2.txt 1.txt > tmp1.txt && ./s21_grep -o "[a-d]" 2.txt 1.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	grep -o '[[:upper:]].*' 1.txt 2.txt > tmp1.txt && ./s21_grep -o '[[:upper:]].*' 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	# Проверка на неколько флагов за раз
		grep -in -f find.txt 1.txt 2.txt > tmp1.txt && ./s21_grep -in -f find.txt 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	# Проверка флага -e
		grep -e wo -e 5 1.txt 2.txt > tmp1.txt && ./s21_grep -e wo -e 5 1.txt 2.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	# Если не вылезло ошибки - [make: *** [test_grep] Error 1], то все тесты пройдены.
	# Удаление временных файлов
	rm tmp1.txt tmp2.txt


debug: s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) $(OBJECT) -std=c11 -o s21_grep -D_DCAT

clean:
	rm -f *.o s21_grep tmp1.txt tmp2.txt test_cat.log log.txt

rebuild: clean all

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
