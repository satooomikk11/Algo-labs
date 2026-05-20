#!/bin/bash

GENERATOR="./generator"
QSORT="./qsort_check"

if [ ! -f "$GENERATOR" ] || [ ! -f "$QSORT" ]; then
    echo "Compile first: make"
    exit 1
fi

echo "Generating small_tests (0-1000, step 50)..."
mkdir -p small_tests
for ((size=0; size<=1000; size+=50)); do
    for ((k=0; k<5; k++)); do
        $GENERATOR $size 1000000 > "small_tests/${size}_${k}.in"
        $QSORT < "small_tests/${size}_${k}.in" > "small_tests/${size}_${k}.out"
    done
    echo "  Size $size done"
done

echo "Generating big_tests (0-1000000, step 10000)..."
mkdir -p big_tests
for ((size=0; size<=1000000; size+=10000)); do
    for ((k=0; k<5; k++)); do
        $GENERATOR $size 1000000 > "big_tests/${size}_${k}.in"
        $QSORT < "big_tests/${size}_${k}.in" > "big_tests/${size}_${k}.out"
    done
    echo "  Size $size done"
done

echo "Generating test_most_dublicates (0-1000000, step 10000)..."
mkdir -p test_most_dublicates
for ((size=0; size<=1000000; size+=10000)); do
    for ((k=0; k<5; k++)); do
        $GENERATOR $size 10000 > "test_most_dublicates/${size}_${k}.in"
        $QSORT < "test_most_dublicates/${size}_${k}.in" > "test_most_dublicates/${size}_${k}.out"
    done
    echo "  Size $size done"
done

echo "All tests generated!"