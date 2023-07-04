#include "workflow/WFFacilities.h"
#include <csignal>
#include "wfrest/HttpServer.h"
#include "wfrest/json.hpp"

using namespace std;
using namespace wfrest;
WFFacilities::WaitGroup wait_group(1);

static void sig_handler(int signo)
{
    wait_group.done(); // 使用静态成员函数时，需要指定具体的 wait_group 对象
}

class WebServer
{

private:
    bool serverInit = false;
    HttpServer svr;
    int seek_ipcam_flag = 0;
    int wait_order_flag = 0;

public:
    WebServer() {}

    void start()
    {

        svr.Static("/", "./file");
        svr.GET("/", [this](const HttpReq *req, HttpResp *resp)
                { this->handleRoot(req, resp); });
        svr.GET("/cameraLive", [this](const HttpReq *req, HttpResp *resp)
                { this->handleCameraLive(req, resp); });
        svr.GET("/test", [this](const HttpReq *req, HttpResp *resp)
                { this->handleTest(req, resp); });
        svr.GET("/login", [this](const HttpReq *req, HttpResp *resp)
                { this->handleLogin(req, resp); });
        svr.POST("/labels", [this](const HttpReq *req, HttpResp *resp)
                 { this->handleLabels(req, resp); });
        svr.GET("/query_has", [this](const HttpReq *req, HttpResp *resp)
                { this->handleQueryHas(req, resp); });

        if (svr.start(8315) == 0)
        {
            cout << "begin web server : url->http://localhost:8315" << endl;
            serverInit = true;
        }
        else
        {
            fprintf(stderr, "Cannot start server");
        }
    }

    void stop()
    {
        if (serverInit)
        {
            cout << "stop web server" << endl;
            serverInit = false;
            svr.stop();
        }
    }

private:
    void handleRoot(const HttpReq *req, HttpResp *resp)
    {
        resp->File("file/templates/login.html");
    }

    void handleCameraLive(const HttpReq *req, HttpResp *resp)
    {
        resp->File("file/templates/cameraLiveButton.html");
    }

    void handleTest(const HttpReq *req, HttpResp *resp)
    {
        resp->File("file/templates/test.html");
    }

    void handleLogin(const HttpReq *req, HttpResp *resp)
    {
        const string &user_name = req->query("username");
        const string &password = req->query("password");
        const string &ip = req->query("ip");
        const string &port = req->query("port");

        cout << user_name << endl;
        cout << password << endl;
        cout << ip << endl;
        cout << port << endl;

        Json json{};
        Json values{};
        values["user_name"] = user_name;
        values["password"] = password;
        values["ip"] = ip;
        values["port"] = port;

        string values_str = values.dump();
        string json_str = "{\"router\":\"login\",\"values\":" + values_str + "}";
        json = Json::parse(json_str);
        string send_to_peer_msg = json.dump();
        cout << "send_to_peer_msg: " << send_to_peer_msg << endl;

        wait_order_flag = 1;
        string order = "yes";
        string input_url = "rtsp://192.168.1.100/live/test";
        cout << "input_url" << input_url << endl;
        while (seek_ipcam_flag != 1)
        {
            seek_ipcam_flag = 1;
        }

        resp->String(input_url);
    }

    void handleLabels(const HttpReq *req, HttpResp *resp)
    {
        cout << "Json" << endl;
        string values_str = req->json<Json>().dump();
        Json json{};
        string json_str = "{\"router\":\"labels\",\"values\":" + values_str + "}";
        json = Json::parse(json_str);
        string send_to_peer_msg = json.dump();
        cout << "send_to_peer_msg: " << send_to_peer_msg << endl;
        resp->Json(req->json<Json>());
    }

    void handleQueryHas(const HttpReq *req, HttpResp *resp)
    {
        if (req->has_query("password"))
        {
            fprintf(stderr, "has password query\n");
        }
        if (req->has_query("info"))
        {
            fprintf(stderr, "has info query\n");
        }
    }
};

int main()
{
    signal(SIGINT, sig_handler);

    WebServer server;
    server.start();
    while (true)
    {
        wait_group.wait();
    }
    return 0;
}
