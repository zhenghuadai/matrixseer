#include <stdio.h>
#include <stdlib.h>
#include "chat_server.h"
int main()
{
    Chat_server s;
    s.init();
    s.run();
}
