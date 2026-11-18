#include <cstdlib>
#include <iostream>
#include "first/first.h"
#include "second/second.h"
#include "third/third.h"
#include "fourth/fourth.h"
#include "fifth/fifth.h"
#include "sixth/sixth.h"
#include "seventh/seventh.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <task number> [args...]" << std::endl;
        return 1;
    }

    switch (std::atoi(argv[1])) {
        case 1:
            return runFirst();
        case 2:
            return runSecond(argc, argv);
        case 3:
            return runThird();
        case 4:
            return runFourth();
        case 5:
            return runFifth();
        case 6:
            return runSixth(argc, argv);
        case 7:
            return runSeventh();
        default:
            std::cerr << "unknown task: " << argv[1] << std::endl;
            return 1;
    }
}
