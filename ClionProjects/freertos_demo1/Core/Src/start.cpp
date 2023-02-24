
#include "start.h"
#include "main.h"
#include <string>
#include "serial_debug.h"
using namespace std;



void startup()
{
    dbg_start(&huart2);

}