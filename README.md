Easy to understand/manage judge core for online judge system (currently only support C/C++)

## compile:

    cmake .

    make

## usage:

mkdir for

* SRC_DIR: a place to hold test src file
* RUN_DIR: work place(for running compiled src file)
* INPUT_DIR: input directory to place the question test case(the input for e.g. std::cin)
* STD_ANSWER_DIR: ground truth answer directory to place the standard answer of the test case.

run:

     ./JudgeCore -c SRC_PATH -d RUN_DIR -l LANG_ENUM -i INPUT_FILE_PATH -a STD_ANSWER_PATH
