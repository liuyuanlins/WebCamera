#include "workflow/WFFacilities.h"
#include <csignal>
#include "wfrest/HttpServer.h"
#include "wfrest/json.hpp"

using namespace std;
using namespace wfrest;

class WebServer : public HttpServer
{

private:
    bool serverInit = false;

    
public:
    WebServer() {}

    void startCameraLiveServer()
    {

        this->Static("/", "./file");
        this->GET("/", [this](const HttpReq *req, HttpResp *resp)
                { this->handleRoot(req, resp); });
        this->GET("/cameraLive", [this](const HttpReq *req, HttpResp *resp)
                { this->handleCameraLive(req, resp); });
        this->GET("/test", [this](const HttpReq *req, HttpResp *resp)
                { this->handleTest(req, resp); });

        this->GET("/query_has", [this](const HttpReq *req, HttpResp *resp)
                { this->handleQueryHas(req, resp); });

        if (this->start(8315) == 0)
        {
            std::cout << "begin web server : url->http://localhost:8315" << endl;
            serverInit = true;
        }
        else
        {
            fprintf(stderr, "Cannot start server");
        }
    }

    void stopCameraLiveServer()
    {
        if (serverInit)
        {
            cout << "stopCameraLiveServer" << endl;
            serverInit = false;
            this->stop();
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
