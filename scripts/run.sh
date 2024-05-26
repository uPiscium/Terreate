if [ $1 == 'c' ]; then
    echo "Clean option selected."
    echo "Cleaning build directory..."
    rm -rf build
    rm -f ../samples/TCTest
    echo "Build directory cleaned."
fi
mkdir -p build
cd build
cmake ..
cmake --build .
cp ./tests/impls/TCTest ../tests/test
cd ../tests
echo "Running TCTest..."
./test
cd ..
