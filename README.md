Easy to understand/manage judge core for online judge system (under process)

## compile:

    cmake .

    make

## usage:

mkdir for

* holding test src file
* work place(for running compiled src file)
* input directory for placeing question test case(the input for e.g. std::cin)
* ground truth answer directory for placing the standard answer of the test case.

run:

     ./JudgeCore -c SRC_PATH -d RUN_DIR -l LANG_ENUM -i INPUT_FILE_PATH -a STD_ANSWER_PATH
