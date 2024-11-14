CC =gcc
CFLAGS =-Wall -Werror -Wextra
OBJECT =s21_cat.c

all: s21_cat

s21_cat: clean s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) $(OBJECT) -std=c11 -o s21_cat

test_cat:
	bash test_s21_cat.sh
	#Проверка на длинные флаги
	./s21_cat -v -t -s --number test_case_cat.txt > tmp1.txt && cat -v -t -s -n test_case_cat.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	./s21_cat -v -t -s --number-nonblank test_case_cat.txt > tmp1.txt && cat -v -t -s -b test_case_cat.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	./s21_cat -v -t --squeeze-blank --number test_case_cat.txt > tmp1.txt && cat -v -t -s -n test_case_cat.txt > tmp2.txt && diff tmp1.txt tmp2.txt
	#Окончание проверки на длинные флаги, если ничего не вывело, то все хорошо
	# Удаление временных файлов
	rm tmp1.txt tmp2.txt


debug: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) $(OBJECT) -std=c11 -o s21_cat -D_DCAT

clean:
	rm -f *.o s21_cat tmp1.txt tmp2.txt test_cat.log log.txt

rebuild: clean all

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
