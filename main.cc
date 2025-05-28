#include <trantor/utils/Logger.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <drogon/drogon.h>

using namespace drogon;

int main()
{
    app().registerHandler(
        "/",
        [](const HttpRequestPtr &request,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            LOG_INFO << "connected:"
                     << (request->connected() ? "true" : "false");
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, World!");
            callback(resp);
        },
        {Get});

    app().registerHandler(
        "/user/{user-name}",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback,
           const std::string &name) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, " + name + "!");
            callback(resp);
        },
        {Get});

    app().registerHandler(
        "/hello?user={user-name}",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback,
           const std::string &name) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("Hello, " + name + "!");
            callback(resp);
        },
        {Get});

    app().registerHandler(
        "/hello_user",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            auto name = req->getOptionalParameter<std::string>("user");
            if (!name)
                resp->setBody("Please tell me your name");
            else
                resp->setBody("Hello, " + name.value() + "!");
            callback(resp);
        },
        {Get});

    app()
        .setBeforeListenSockOptCallback([](int fd) {
            LOG_INFO << "setBeforeListenSockOptCallback:" << fd;
            int enable = 1;
            if (setsockopt(
                    fd, IPPROTO_TCP, TCP_FASTOPEN, &enable, sizeof(enable)) ==
                -1)
            {
                LOG_INFO << "setsockopt TCP_FASTOPEN failed";
            }
        })
        .setAfterAcceptSockOptCallback([](int) {});

    LOG_INFO << "Server running on 127.0.0.1:3031";
    app().addListener("127.0.0.1", 3031).run();
}