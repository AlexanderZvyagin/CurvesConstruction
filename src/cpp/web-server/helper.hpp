#pragma once
#include <iostream>
#include <libwebsockets.h>

class LibWebSocketCallBackReason {
public:
    int number;
    const char *short_description;
    const char *long_description;
    bool operator < (const LibWebSocketCallBackReason &x) const {return number<x.number;}
};

const LibWebSocketCallBackReason & lws_cb_reason(int number);
// void lws_print_reason(int number);

std::ostream & operator << (std::ostream &os, const LibWebSocketCallBackReason &x);
