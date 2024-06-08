#!/bin/bash

STUDENT_BIN=42sh

COMMAND_FOLDER=command
RESULT_FOLDER=result

make -s -C ../ && cp ../$STUDENT_BIN . && make -s -C ../ fclean

# verify that the STUDENT_BIN exists
if [ ! -f $STUDENT_BIN ]; then
    echo "ERROR: $STUDENT_BIN not found"
    exit 84
fi
# verify that the STUDENT_BIN is executable
if [ ! -x $STUDENT_BIN ]; then
    echo "ERROR: $STUDENT_BIN is not executable"
    rm -f $STUDENT_BIN
    exit 84
fi
# verify that the COMMAND_FOLDER exists
if [ ! -d $COMMAND_FOLDER ]; then
    echo "ERROR: $COMMAND_FOLDER not found"
    rm -f $STUDENT_BIN
    exit 84
fi
# verify that the COMMAND_FOLDER is not empty
if [ ! "$(ls -A $COMMAND_FOLDER)" ]; then
    echo "ERROR: $COMMAND_FOLDER is empty"
    rm -f $STUDENT_BIN
    exit 84
fi

# verify if the result folder exists
if [ ! -d $RESULT_FOLDER ]; then
    mkdir $RESULT_FOLDER
else
    rm -f $RESULT_FOLDER/*
fi
NB_TESTS=0
TEST_PASSED=0
IGNORED=0

for Test in $(ls -a $COMMAND_FOLDER); do
    # skip the . and .. folders
    if [ $Test == "." ] || [ $Test == ".." ]; then
        continue
    fi
    # If the filename start with a dot, skip it and increment the IGNORED counter
    if [[ $Test == .* ]]; then
        IGNORED=$((IGNORED+1))
        continue
    fi
    IS_FAIL=0
    COUNT=0
    TOTAL_COUNT=0
    # Lire les commandes depuis le fichier en ignorant les lignes vides et celles commençant par '#'
    while IFS= read -r line; do
        # Ignorer les lignes vides
        if [ -z "$line" ]; then
            continue
        fi

        # Ignorer les lignes commençant par '#'
        if [[ "$line" == "#"* ]]; then
            continue
        fi

        # Exécuter la commande et comparer les sorties
        echo -ne $line | ./$STUDENT_BIN > student_output.txt 2> /dev/null
        echo -ne $line | tcsh > reference_output.txt 2> /dev/null

        # Compare the student_output.txt and reference_output.txt if there is an error exit the loop for
        diff_output=$(diff student_output.txt reference_output.txt)

        if [ $? -ne 0 ]; then
            echo -e "\e[31mFailed --> $line\e[0m"
            # Si le fichier de résultat n'existe pas, le créer
            if [ ! -f $RESULT_FOLDER/result_$Test ]; then
                touch $RESULT_FOLDER/result_$Test
            else
                echo "======================================================================" >> $RESULT_FOLDER/result_$Test
            fi
            # Écrire le résultat dans le fichier
            echo "Student output:" >> $RESULT_FOLDER/result_$Test
            cat student_output.txt >> $RESULT_FOLDER/result_$Test
            echo "" >> $RESULT_FOLDER/result_$Test
            echo "Reference output:" >> $RESULT_FOLDER/result_$Test
            cat reference_output.txt >> $RESULT_FOLDER/result_$Test
            IS_FAIL=1
        else 
            TEST_PASSED=$((TEST_PASSED+1))
            COUNT=$((COUNT+1))
        fi
        NB_TESTS=$((NB_TESTS+1))
        TOTAL_COUNT=$((TOTAL_COUNT+1))
    done < $COMMAND_FOLDER/$Test

    PERCENTAGE=$((COUNT * 100 / TOTAL_COUNT))
    if [ $IS_FAIL -eq 0 ]; then
        printf "%-25s \e[32mPassed (%d%%)\e[0m\n" $Test $PERCENTAGE
    else
        printf "%-25s \e[31mFailed (%d%%)\e[0m\n" $Test $PERCENTAGE
    fi
done

echo -e "======================================="
if [ $TEST_PASSED -eq $NB_TESTS ]; then
    echo -e "\e[32m$TEST_PASSED/$NB_TESTS tests passed\e[0m"
else
    echo -e "\e[31m$TEST_PASSED/$NB_TESTS tests passed\e[0m"
fi
echo -e "======================================="
rm -f $STUDENT_BIN
exit 0
