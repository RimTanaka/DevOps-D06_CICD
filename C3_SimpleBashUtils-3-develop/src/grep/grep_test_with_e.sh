COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE1="../grep/test1.txt"
TEST_FILE2="../grep/test2.txt"
PATTERN_FILE="../grep/reg_exmpl.txt"
GREP_FILE="../grep/s21_grep"
arguments=(-i -v -c -l -n)

TEST1="cat $TEST_FILE1"
#echo "GREP TEST 1: $TEST1"
$GREP_FILE $TEST1 > s21_grep.txt
grep $TEST1 > grep.txt
DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST1 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1 - FAIL"
              (( COUNTER_FAIL++ ))
          fi

TEST2="-e cat $TEST_FILE1"
#echo "GREP TEST 2: $TEST2"
$GREP_FILE $TEST2 > s21_grep.txt
grep $TEST2 > grep.txt
DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST2 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2 - FAIL"
              (( COUNTER_FAIL++ ))
          fi

TEST3="-e cat -e grep $TEST_FILE1"
#echo "GREP TEST 3: $TEST3"
$GREP_FILE $TEST3 > s21_grep.txt
grep $TEST3 > grep.txt
DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST3 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST3 - FAIL"
              (( COUNTER_FAIL++ ))
          fi

TEST4="-e cat -e grep $TEST_FILE1 $TEST_FILE2"
#echo "GREP TEST 4: $TEST4"
$GREP_FILE $TEST4 > s21_grep.txt
grep $TEST4 > grep.txt
DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST4 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST4 - FAIL"
              (( COUNTER_FAIL++ ))
          fi

for var in ${arguments[@]}
do
          TEST5="$var cat $TEST_FILE1"
          #echo "GREP TEST 5 $TEST5"
          $GREP_FILE $TEST5 > s21_grep.txt
          grep $TEST5 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST5 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST5 - FAIL"
              (( COUNTER_FAIL++ ))
          fi
done

for var in ${arguments[@]}
do
          TEST9="$var -e less -e grep $TEST_FILE1 $TEST_FILE2"
          #echo "GREP TEST 9 $TEST9"
          $GREP_FILE $TEST9 > s21_grep.txt
          grep $TEST9 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              echo "$TEST9 - SUCCESS"
      		    (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST9 - FAIL"
              (( COUNTER_FAIL++ ))
          fi
done


#rm s21_grep.txt grep.txt
echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

 rm s21_grep.txt
 rm grep.txt
