#pragma once

#include <libwebsockets.h>

int
callback_protocol(struct lws *wsi, enum lws_callback_reasons reason,
            void *user, void *in, size_t len);

class UserData;
template<typename> class Session;

class SessionWrapper {
public:
    Session<UserData> *session;
};

// class UserSessionData {
// public:
//     Session *session;
//     UserData *data;
// };
